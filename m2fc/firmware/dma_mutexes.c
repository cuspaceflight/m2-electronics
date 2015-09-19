#include "dma_mutexes.h"

Mutex dma1_stream0_mutex;

void dma_mutexes_init(void)
{
    chMtxInit(&dma1_stream0_mutex);
}
