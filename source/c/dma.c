#include "stm32f103xe.h"
#include "dma.h"


void DMA_init(uint16_t *dma_buff, uint16_t dma_buff_size)
{
	DMA1_Channel1->CCR = 0;
	
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
	DMA1_Channel1->CMAR = (uint32_t) dma_buff;
	DMA1_Channel1->CNDTR = dma_buff_size;
	DMA1_Channel1->CPAR = (uint32_t) 0x4001244C;
	DMA1_Channel1->CCR &= ~DMA_CCR_DIR;
	DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_CIRC;
	DMA1_Channel1->CCR &=~ DMA_CCR_PINC;
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
	DMA1_Channel1->CCR |= DMA_CCR_PL;
	DMA1_Channel1->CCR |= DMA_CCR_EN;

}