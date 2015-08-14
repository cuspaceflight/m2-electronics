#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ldpc_ecc.h"

int main(int argc, char* argv[])
{
    uint8_t data_tx[16], coded_tx[32];
    int i;
    (void)argc;
    (void)argv;

    srand(time(NULL));
    for(i=0; i<16; i++) data_tx[i] = rand() & 0xFF;

    printf("Data:    ");
    for(i=0; i<16; i++) printf(" %02X", data_tx[i]);

    ldpc_encode(data_tx, coded_tx);

    printf("\nCodeword:");
    for(i=0; i<32; i++) printf(" %02X", coded_tx[i]);
    printf("\n");

    return 0;
}
