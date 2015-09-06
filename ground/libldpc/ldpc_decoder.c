#include <math.h>
#include <stdio.h>
#include "ldpc_decoder.h"

/* Maximum number of iterations to attempt to decode a message for. */
#define MAX_ITERS (100)

/* Hard decode the systematic message bits in `llrs`, packing them MSb first
 * into `out`.
 */
static void pack_message(double llrs[N], uint8_t out[K/8])
{
    unsigned int i;
    
    for(i=0; i<K/8; i++)
        out[i] = 0x00;

    for(i=0; i<N/2; i++)
        if(llrs[i] <= 0.0)
            out[i/8] |= 1 << (7 - (i % 8));
}

void ldpc_init()
{
    parity_matrix_init();
}

#include <string.h>

extern bool ldpc_decode(double llrs[N], uint8_t out[K/8])
{
    double u[K][N];
    double v[N][K];
    double r[N];
    int iter, i, j, a, b, idx1, idx2;

    /* Copy llrs to r and to v_(a->i). */
    for(a=0; a<N; a++) {
        r[a] = llrs[a];
        /*for(i=0; i<K; i++)*/
            /*v[a][i] = llrs[a];*/
        /*for(idx1=0; idx1<parity_col_degs[a]; idx1++) {*/
            /*i = parity_col_cons[a][idx1];*/
            /*v[a][i] = llrs[a];*/
        /*}*/
    }

    for(a=0; a<N; a++)
        for(i=0; i<K; i++)
            v[a][i] = 0.0f;

    for(iter=0; iter<MAX_ITERS; iter++) {
        /* First check if we succeeded in decoding,
         * and if so then pack the message and return it.
         */
        if(parity_matrix_check(r)) {
            pack_message(r, out);
            return true;
        }

        /* Next compute the check node to variable node messages,
         * u_(i->a) = 2 atanh( prod_[b!=a]( tanh( v_(b->i)/2 ) ) )
         */
        for(i=0; i<K; i++) {
            for(idx1=0; idx1<parity_row_degs[i]; idx1++) {
                a = parity_row_cons[i][idx1];
                u[i][a] = 1.0f;
                for(idx2=0; idx2<parity_row_degs[i]; idx2++) {
                    b = parity_row_cons[i][idx2];
                    if(b != a)
                        u[i][a] *= tanh(v[b][i] / 2.0f);
                }
                u[i][a] = 2.0f * atanh(u[i][a]);
            }
        }

        /* Finally compute the variable node to check node messages,
         * v_(a->i) = LLRs_a + sum_[j!=i](u_(j->a)),
         * and the current marginals,
         * r_a = LLRs_a + sum_[i] u_(i->a)
         */
        for(a=0; a<N; a++) {
            for(idx1=0; idx1<parity_col_degs[a]; idx1++) {
                i = parity_col_cons[a][idx1];
                v[a][i] = llrs[a];
                for(idx2=0; idx2<parity_col_degs[a]; idx2++) {
                    j = parity_col_cons[a][idx2];
                    if(j != i)
                        v[a][i] += u[j][a];
                }
            }

            r[a] = llrs[a];
            for(i=0; i<K; i++)
                r[a] += u[i][a];
        }
    }

    /* If we hit the iteration limit, we failed to decode. */
    return false;
}
