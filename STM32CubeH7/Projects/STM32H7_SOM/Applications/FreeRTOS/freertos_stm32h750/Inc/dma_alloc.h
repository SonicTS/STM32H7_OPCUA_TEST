#ifndef __DMA_ALLOC_H
#define __DMA_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "lcd.h"

#define DMA_FB_START		(0xD0000000UL)
#define DMA_FB_SIZE			(LCD_MAX_WIDTH * LCD_MAX_HIGHT * 4)
#define DMA_REGION_START	(DMA_FB_START + (DMA_FB_SIZE * 2))
#define DMA_REGION_SIZE		((4 * 1024 * 1024) - (DMA_FB_SIZE * 2))

void define_dma_regions(const HeapRegion_t * const pxHeapRegions);
void * dma_alloc(size_t xSize);
void dma_free(void *pv);

#ifdef __cplusplus
}
#endif

#endif /* __DMA_ALLOC_H */
