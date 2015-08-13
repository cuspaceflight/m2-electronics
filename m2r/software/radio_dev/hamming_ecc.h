#ifndef HAMMING_ECC_H
#define HAMMING_ECC_H

#include <stdint.h>

/*
 * For each of 16 bytes in `data`, write two bytes to `coded`,
 * where the first coded byte is the codeword for the most significant nibble
 * of data byte, and the second coded byte is the codeword for the least
 * significant nibble. Sets the MSb of each byte in `coded` to 0.
 */
void data_to_coded(uint8_t* data, uint8_t* coded);

/*
 * For each two bytes in `coded`, extract one byte into `data`, correcting up
 * to one bit error per byte in `coded`. Ignores the MSb of each byte in
 * `coded`.
 */
void coded_to_data(uint8_t* coded, uint8_t* data);

/*
 * Interleave 32 bytes (all with MSb=0, i.e. 7bit) from `coded` into
 * 28 bytes in `interleaved`.
 * The first 4 bytes in `interleaved` contain bit 0 of each byte in `coded`,
 * with the very first byte in `interleaved` containing bit 0 (LSb) of the
 * first eight bytes from `coded`.
 * The next 4 bytes in `interleaved` contain bit 1 from each byte in `coded`,
 * again with the 5th byte containing bits from the first eight bytes in
 * `coded`, the 6th byte containing bits from the next eight bytes, etc.
 * NOTE: Endienness-sensitive, requires LE.
 */
void coded_to_interleaved(uint8_t* coded, uint8_t* interleaved);

/*
 * Deinterleaves 28 bytes from `interleaved` into 32 bytes in `coded`.
 * See coded_to_interleaved for description.
 */
void interleaved_to_coded(uint8_t* interleaved, uint8_t* coded);

#endif /* HAMMING_ECC_H */
