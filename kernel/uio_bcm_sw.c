/*
 * @file uio_bcm_sw.c -- UIO driver for Broadcom Switch chips.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) luxebeng <luxebeng@gmail.com>, 2014
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/uio_driver.h>
#include <linux/spinlock.h>
#include <linux/bitops.h>


typedef struct {
    struct uio_info *uio_info;
    spinlock_t      sw_lock;
    unsigned long   flags;
} bcm_sw_priv_t, *pbcm_sw_priv_t;

static irqreturn_t bcm_sw_irq(int irq, struct uio_info *dev_info)
{
    pbcm_sw_priv_t     priv;

    priv = dev_info->priv;
    /* CPLD will take care of the interrupt */
    if (!test_and_set_bit(0, &priv->flags))
        disable_irq_nosync(irq);

    return IRQ_NONE;
}

static int bcm_sw_irqcontrol(struct uio_info *dev_info, s32 irq_on)
{
    pbcm_sw_priv_t priv = (pbcm_sw_priv_t)dev_info->priv;
    unsigned long flags;

    /* Allow user space to enable and disable the interrupt
     * in the interrupt controller, but keep track of the
     * state to prevent per-irq depth damage.
     *
     * Serialize this operation to support multiple tasks.
     */

    spin_lock_irqsave(&priv->sw_lock, flags);
    if (irq_on) {
        if (test_and_clear_bit(0, &priv->flags))
            enable_irq(dev_info->irq);
    } else {
        if (!test_and_set_bit(0, &priv->flags))
            disable_irq(dev_info->irq);
    }
    spin_unlock_irqrestore(&priv->sw_lock, flags);

    return 0;
}

static int bcm_sw_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    struct uio_info *info;
    pbcm_sw_priv_t  priv;
    int ret;

    info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
    if (!info)
        return -ENOMEM;
    priv = kzalloc(sizeof(*priv), GFP_KERNEL);
    if (!priv) {
        kfree(info);
        return -ENOMEM;
    }

    if (pci_enable_device(pdev))
        goto out_free;

    if (pci_request_regions(pdev, "bcm_sw"))
        goto out_disable;

    priv->uio_info = info;
    priv->flags = 0; /* interrupt is enabled to begin with */
    spin_lock_init(&priv->sw_lock);

    info->name = "bcm_sw";
    info->version = "0.0.1";
    info->mem[0].addr = pci_resource_start(pdev, 0);
    if (!info->mem[0].addr)
        goto out_release;

    info->mem[0].internal_addr = pci_ioremap_bar(pdev,0);
    if (!info->mem[0].internal_addr)
        goto out_release;

    info->mem[0].size = pci_resource_len(pdev, 0);
    info->mem[0].memtype = UIO_MEM_PHYS;
    info->mem[1].addr = pci_resource_start(pdev, 2);
    info->mem[1].size = pci_resource_len(pdev, 2);
    info->mem[1].memtype = UIO_MEM_PHYS;

    info->irq        = pdev->irq;
    info->irq_flags  = IRQF_SHARED;
    info->handler    = bcm_sw_irq;
    info->irqcontrol = bcm_sw_irqcontrol;
    info->priv       = priv;

    ret = uio_register_device(&pdev->dev, info);
    if (ret)
        goto out_unmap;

    pci_set_drvdata(pdev, info);

    return 0;

out_unmap:
    iounmap(info->mem[0].internal_addr);
out_release:
    pci_release_regions(pdev);
out_disable:
    pci_disable_device(pdev);
out_free:
    kfree(priv);
    kfree(info);
    return -ENODEV;
}

static void bcm_sw_remove(struct pci_dev *pdev)
{
    struct uio_info *info = pci_get_drvdata(pdev);

    uio_unregister_device(info);
    pci_release_regions(pdev);
    pci_disable_device(pdev);
    pci_set_drvdata(pdev, NULL);
    iounmap(info->priv);

    kfree(info->priv);
    kfree(info);
}

#define PCI_DEVICE_ID_BCM56342A   0xb342
static struct pci_device_id sw_ids[] __devinitdata = {
    { PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, PCI_DEVICE_ID_BCM56342A), },
    { 0, }
};

static struct pci_driver bcm_sw_pci_driver = {
    .name = "bcm_sw",
    .id_table = sw_ids,
    .probe = bcm_sw_probe,
    .remove = bcm_sw_remove,
};

static int __init bcm_sw_init(void)
{
    return pci_register_driver(&bcm_sw_pci_driver);
}

static void __exit bcm_sw_exit(void)
{
    pci_unregister_driver(&bcm_sw_pci_driver);
}

MODULE_DEVICE_TABLE(pci, sw_ids);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("luxebeng@gmail.com");

module_init(bcm_sw_init);
module_exit(bcm_sw_exit);
