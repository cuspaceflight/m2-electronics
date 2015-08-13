#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hamming_ecc.h"

int main(int argc, char* argv[]) {
    size_t i;
    uint8_t data_tx[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint8_t data_rx[16];
    uint8_t coded_tx[32], coded_rx[32];
    uint8_t interleaved_tx[28], interleaved_rx[28];

    (void)argc;
    (void)argv;

    printf("Transmitting:\n");
    for(i=0; i<sizeof(data_tx); i++) {
        printf("0x%02X ", data_tx[i]);
    }
    printf("\n");

    data_to_coded(data_tx, coded_tx);
    coded_to_interleaved(coded_tx, interleaved_tx);
    memcpy(interleaved_rx, interleaved_rx, sizeof(interleaved_rx));
    interleaved_to_coded(interleaved_rx, coded_rx);
    coded_to_data(coded_rx, data_rx);

    printf("Received:\n");
    for(i=0; i<sizeof(data_rx); i++) {
        printf("0x%02X ", data_rx[i]);
    }
    printf("\n");

    return 0;
}
