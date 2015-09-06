#include "parity_matrix.h"

/* Size of parity matrix component blocks */
#define M (32)

/* Blocks per row */
#define BR (K/M)

/* Blocks per column */
#define BC (N/M)

/* Storage for H and connection representation. */
bool parity_H[K][N];
int parity_row_degs[K];
int parity_row_cons[K][N];
int parity_col_degs[N];
int parity_col_cons[N][K];

/* Store a sub-block.
 * Mainly used so functions can return sub-blocks.
 */
typedef struct {
    bool h[M][M];
} Block;

/* Generate 0_M, the all-zeros block. */
static Block z()
{
    int i, j;
    Block b;
    for(i=0; i<M; i++)
        for(j=0; j<M; j++)
            b.h[i][j] = 0;
    return b;
}

/* Generate Phi_M(N), the nth right circular shift of the MxM identity matrix.
 */
static Block p(int n)
{
    int i, j;
    Block b;
    for(i=0; i<M; i++)
        for(j=0; j<M; j++)
            b.h[i][j] = (j == (i + n) % M);
    return b;
}

/* Generate I_M, the MxM identity matrix. */
static Block e()
{
    return p(0);
}

/* Generate I_M + Phi_M(n) mod 2. */
static Block s(int n)
{
    int i, j;
    Block eye = e(), phi = p(n), b;
    for(i=0; i<M; i++)
        for(j=0; j<M; j++)
            b.h[i][j] = phi.h[i][j] ^ eye.h[i][j];
    return b;
}

void parity_matrix_init()
{
    /* Parity check definition by sub-block from H_128x256,
     * defined in CCSDS 231.1-O-1 p2-2.
     */
    Block subblocks[BR][BC] = {
        {s(31), p(15), p(25), p(0) , z()  , p(20), p(12), e()  },
        {p(28), s(30), p(29), p(24), e()  , z()  , p(1) , p(20)},
        {p(8) , p(0),  s(28), p(1) , p(29), e()  , z()  , p(21)},
        {p(18), p(30), p(0) , s(30), p(25), p(26), e()  , z()  }
    };

    /* Build up H from the sub-blocks in h. */
    int i, a, x, y;
    for(i=0; i<BR; i++)
        for(a=0; a<BC; a++)
            for(x=0; x<M; x++)
                for(y=0; y<M; y++)
                    parity_H[i*M + x][a*M + y] = subblocks[i][a].h[x][y];

    /* Compute connection representation. */
    for(i=0; i<K; i++) {
        parity_row_degs[i] = 0;

        for(a=0; a<N; a++) {
            if(i==0) parity_col_degs[a] = 0;

            /* If there's a connection, store it in the next slot in the
             * respective arrays, and then increment the degrees.
             */
            if(parity_H[i][a]) {
                parity_row_cons[i][parity_row_degs[i]] = a;
                parity_col_cons[a][parity_col_degs[a]] = i;
                parity_row_degs[i]++;
                parity_col_degs[a]++;
            }
        }
    }
}

bool parity_matrix_check(double x[N])
{
    int i, a, parity;

    /* For each parity check equation */
    for(i=0; i<K; i++) {
        parity = 0;

        /* For each code bit */
        for(a=0; a<N; a++) {
            /* If the equation covers this bit, and the bit is hard-decoded 1,
             * then increment parity.
             */
            if(parity_H[i][a] && x[a] <= 0.0)
                parity++;
        }

        /* Immediately return false when an equation is not satisfied. */
        if(parity % 2 != 0)
            return false;
    }

    /* If all equations were satisfied then x hard-decodes to a codeword. */
    return true;
}
