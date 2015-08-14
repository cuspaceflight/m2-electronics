#ifndef LDPC_ECC_H
#define LDPC_ECC_H

#include <stdint.h>

/* Encode 16 bytes of `data` to 32 bytes of `coded`. */
void ldpc_encode(uint8_t* data, uint8_t* coded);

/* Decode 256 LLRs into 32 bytes of codeword in `coded`. */
void ldpc_decode(int8_t* llrs, uint8_t* coded);

#endif /* LDPC_ECC_H */
