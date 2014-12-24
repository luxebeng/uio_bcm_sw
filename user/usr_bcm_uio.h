/*
 *   @file usr_bcm_uio.h -- UIO code for bcm.
 *
 *   @brief
 *    This program is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License version 2 as published
 *    by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc., 59
 *    Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __JBCM_UIO_H_
#define __JBCM_UIO_H_

#define DMA_MEM_ALLOCATE_MMAP_OFFSET 0xFFFFF000UL

const char *bcm_devm_name="bcm_sw";

/**
 * the irq handler for BCM UIO
 * @param data
 *    data is used by interupt handler.
 * @return
 *   - None
 */
extern void  jbcm_sw_irq_handler(void *data);
/**
 * get base address for the BCM
 * @param pic_id
 *    the board I2C id.
 * @return
 *    the base address
 */
extern uint32_t *jbcm_sw_baseaddr_get(pic_id_t pic_id);
/**
 * get base address for DMA pool
 * @param size
 *    the DMA pool size.
 * @return
 *    the base address for the DMA pool.
 */
extern uint32_t *jbcm_dma_region_alloc(u_int32_t size);

#endif
