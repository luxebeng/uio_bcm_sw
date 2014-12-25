/*
 *   @file usr_bcm_uio.c -- UIO user space code for bcm.
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

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sched.h>
#include <limits.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "jbcm_pub.h"
#include <sal/core/spl.h>
#include <sal/appl/pci.h>
#include <ibde.h>
#include <infra/uio_user_common.h>
#include "jbcm_uio.h"

uint32_t *
jbcm_sw_baseaddr_get(pic_id_t pic_id)
{
    int32_t fd;
    uint32_t * base_addr;
    char uio_name[PATH_MAX];

    if (false == uio_get_uio_name(bcm_devm_name, uio_name))
    {
        syslog(LOG_ERR, "Can't find switch UIO device\n", __FUNCTION__);
        return NULL;
    }

    fd = open(uio_name, O_RDWR);
    if (fd < 0) {

        syslog(LOG_ERR, "Can't open switch UIO device\n", __FUNCTION__);
        return NULL;
    }

    base_addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);

    return base_addr;
}

static void
isr_handlers(void)
{
    /*TODO check in the code with jbcm_bde.c*/
    return;
}

void
jbcm_sw_irq_handler(void *data)
{
    int32_t fd;
    uint32_t   event_count;
    uint32_t   count;
    uint32_t   irq_on;
    char uio_name[PATH_MAX];

    if (uio_get_uio_name(bcm_devm_name, uio_name)== false)
    {
        syslog(LOG_ERR, "Can't find switch UIO device\n", __FUNCTION__);
        return;
    }

    fd = open(uio_name, O_RDWR);
    if (fd < 0)
    {
        syslog(LOG_ERR, "Can't open switch UIO device\n", __FUNCTION__);
        return;
    }

    do {
        count = read(fd, &event_count, sizeof(event_count));

        if (sizeof(uint32_t) != count) {
            syslog(LOG_ERR, "Read UIO event error \n", __FUNCTION__);
            continue;
        }

        if (event_count)
        {
            /* Hook with BCM SDK interrupt handler */
            isr_handlers();
        }
       /* Renable interrupt */
        irq_on = 1;
        write(fd, &irq_on, sizeof(uint32_t));
    } while (1);

}

uint32_t *
jbcm_dma_region_alloc(uint32_t size)
{
    int32_t fd;
    uint32_t * dma_region_addr;
    char uio_name[PATH_MAX];

    if (uio_get_uio_name(bcm_devm_name, uio_name)== false)
    {
        syslog(LOG_ERR, "Can't find switch UIO device\n", __FUNCTION__);
        return NULL;
    }

    fd = open(uio_name, O_RDWR);
    if (fd < 0)
    {
        syslog(LOG_ERR, "Can't open switch UIO device\n", __FUNCTION__);
        return NULL;
    }

    dma_region_addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                           fd, DMA_MEM_ALLOCATE_MMAP_OFFSET);

    return dma_region_addr;
}

