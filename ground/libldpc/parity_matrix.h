/* Parity matrix creation */

#ifndef _PARITY_MATRIX_H_
#define _PARITY_MATRIX_H_

#include <stdbool.h>

/* Code dimension */
#define K (128)

/* Code length */
#define N (256)

/* Parity matrix */
extern bool parity_H[K][N];

/* Alternative representation: arrays of connected indicies.
 * parity_row_degs[i] gives the degree of row i,
 * parity_row_cons[i] is a degree-long array of indices of columns
 *                    that row i is connected to,
 * and likewise for parity_col_degs and parity_col_cons.
 */
extern int parity_row_degs[K];
extern int parity_row_cons[K][N];
extern int parity_col_degs[N];
extern int parity_col_cons[N][K];

/* Generate the parity matrix. Call once at startup. */
void parity_matrix_init(void);

/* Check if x is a codeword (i.e., x.H==0),
 * where x is an array of LLRs with positive values implying 0 is more likely.
 */
bool parity_matrix_check(double x[N]);

#endif /* _PARITY_MATRIX_H_ */
