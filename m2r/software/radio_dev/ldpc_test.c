#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ldpc_ecc.h"

int main(int argc, char* argv[])
{
    uint8_t data_tx[16], coded_tx[32], coded_rx[32], data_rx[16];
    int8_t llrs[256];
    int i;
    (void)argc;
    (void)argv;

    srand(time(NULL));
    for(i=0; i<16; i++) data_tx[i] = rand() & 0xFF;

    printf("TX Data:    ");
    for(i=0; i<16; i++) printf(" 0x%02X,", data_tx[i]);

    ldpc_encode(data_tx, coded_tx);

    printf("\nTX Codeword:\n");
    for(i=0; i<32; i++) printf("%02X ", coded_tx[i]);
    printf("\n");

    for(i=0; i<256; i++) {
        uint8_t coded_byte = i / 8;
        uint8_t coded_bit = 7 - (i % 8);
        uint8_t bit = (coded_tx[coded_byte] & (1<<coded_bit)) >> coded_bit;
        if(bit)
            llrs[i] = 100;
        else
            llrs[i] = -100;
    }

    ldpc_decode(llrs, coded_rx);

    printf("\nRX Codeword:");
    for(i=0; i<32; i++) printf(" %02X", coded_rx[i]);
    printf("\n");

    printf("\nRX Data:    ");
    for(i=0; i<16; i++) printf(" %02X", data_rx[i]);
    printf("\n");

    return 0;
}
