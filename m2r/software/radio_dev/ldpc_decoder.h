#ifndef LDPC_DECODER_H
#define LDPC_DECODER_H

#include <stdint.h>

/* Decode 256 LLRs into 32 bytes of codeword in `coded`. */
void ldpc_decode(double* llrs, uint8_t* coded);

#endif /* LDPC_DECODER_H */
