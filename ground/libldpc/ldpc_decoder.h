/* LDPC decoder */

#ifndef _LDPC_DECODER_H_
#define _LDPC_DECODER_H_

#include <stdint.h>
#include "parity_matrix.h"

/* Initialise the LDPC decoder. Call once at startup. */
void ldpc_init(void);

/* Decode a message from the LLRs in `llrs`, where positive values are more
 * likely to be 0, and write the result in `out` as packed bytes, MSb first.
 * Returns true on success and false on failure.
 */
extern bool ldpc_decode(double llrs[N], uint8_t out[K/8]);

#endif /* _LDPC_DECODER_H_ */
