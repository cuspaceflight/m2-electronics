/*
 * Mutex lock for DMAs
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#ifndef DMA_MUTEXES_H
#define DMA_MUTEXES_H

#include <ch.h>
void dma_mutexes_init(void);
extern Mutex dma1_stream0_mutex;

#endif /*DMA_MUTEXES_H*/
