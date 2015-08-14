#include "ldpc_decoder.h"
#include "ldpc_parity_check.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* Check if x.H=0 implying that we have finished. */
static bool ldpc_check_if_codeword(uint8_t* coded)
{
    int i, a;
    /* For each parity check equation */
    for(i=0; i<128; i++) {
        int parity = 0;

        /* For each coded bit */
        for(a=0; a<256; a++) {
            uint8_t bit = (coded[a/8] >> (7 - (a%8))) & 1;
            parity += bit & ldpc_parity[i][a];
        }

        /* Return false at the first violated parity check. */
        if(parity % 2 != 0)
            return false;
    }

    return true;
}

static inline double sign(double x)
{
    return (x > 0) - (x < 0);
}

static inline double minstar(double a, double b)
{
    return sign(a) * sign(b) * fmin(fabs(a), fabs(b))
        /*
            + log(1 + exp(-(fabs(a) + fabs(b))))
            + log(1 + exp(-(fabs(a) - fabs(b))))
        */
        ;
}

/* Decode 256 LLRs into 32 bytes of codeword in `coded`. */
void ldpc_decode(double* r, uint8_t* coded)
{
    int iter, a, b, i, j;
    double **u, **v, *llrs;
    const int max_iters = 100;

    /* Check if we can return early. */
    memset(coded, 0, 32);
    for(i=0; i<256; i++) {
        coded[i/8] |= (r[i] < 0.0f) << (7 - (i % 8));
    }
    if(ldpc_check_if_codeword(coded)) {
        /*printf("Codeword already valid, returning.\n");*/
        return;
    }

    /* Allocate memory for u i->a */
    u = (double**)malloc(128 * sizeof(double*));
    u[0] = (double*)malloc(128 * 256 * sizeof(double));
    for(i=1; i<128; i++) {
        u[i] = u[0] + i * 256;
    }

    /* Allocate memory for v a->i */
    v = (double**)malloc(256 * sizeof(double*));
    v[0] = (double*)malloc(256 * 128 * sizeof(double));
    for(a=1; a<256; a++) {
        v[a] = v[0] + a * 128;
    }

    /* Allocate memory for the LLRs */
    llrs = (double*)malloc(256 * sizeof(double));

    /* Initialisation */
    for(a=0; a<256; a++) {
        llrs[a] = r[a];
        for(i=0; i<128; i++) {
            if(!ldpc_parity[i][a]) continue;
            v[a][i] = r[a];
        }
    }

    /*printf("[00] LLR[0]=%f\n", llrs[0]);*/

    /* Cap max iterations. */
    for(iter=0; iter<max_iters; iter++) {
        /* Check nodes to variable nodes */
        for(i=0; i<128; i++) {
            for(a=0; a<256; a++) {
                if(!ldpc_parity[i][a]) continue;
                /*u[i][a] = minstar(v[0][i], v[1][i]);*/
                u[i][a] = 1.0f;
                for(b=2; b<256; b++) {
                    if(!ldpc_parity[i][b] || b==a) continue;
                    /*u[i][a] = minstar(u[i][a], v[b][i]);*/
                    u[i][a] *= tanh(v[b][i] / 2.0f);
                }
                u[i][a] = 2.0f * atanh(u[i][a]);
            }
        }

        /* Check if we're done. */
        memset(coded, 0, 32);
        for(a=0; a<256; a++) {
            llrs[a] = r[a];
            for(i=0; i<128; i++) {
                llrs[a] += u[i][a];
            }
            coded[a/8] |= (llrs[a] <= 0.0) << (7 - (a % 8));
        }
        /*printf("[%02d] LLR[0]=%f\n", iter, llrs[0]);*/
        if(ldpc_check_if_codeword(coded)) {
            /*printf("Codeword found after %d iters, returning.\n", iter);*/
            break;
        }

        /* Variable nodes to check nodes */
        for(a=0; a<256; a++) {
            for(i=0; i<128; i++) {
                if(!ldpc_parity[i][a]) continue;
                v[a][i] = r[a];
                for(j=0; j<128; j++) {
                    if(!ldpc_parity[j][a] || j==i) continue;
                    v[a][i] += u[j][a];
                }
            }
        }

    }

    if(iter == max_iters) {
        /*printf("Max iterations exceeded, returning.\n");*/
    }

    free(v[0]);
    free(v);
    free(u[0]);
    free(u);
    free(llrs);
}
