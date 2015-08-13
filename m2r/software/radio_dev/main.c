#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hamming_ecc.h"

bool run_trial(double ber, unsigned int n_errors, size_t *nerrs) {
    size_t i, j;
    uint8_t data_tx[16], data_rx[16];
    uint8_t coded_tx[32], coded_rx[32];
    uint8_t interleaved_tx[28], interleaved_rx[28];

    for(i=0; i<sizeof(data_tx); i++) {
        data_tx[i] = rand() & 0xFF;
    }

    data_to_coded(data_tx, coded_tx);
    coded_to_interleaved(coded_tx, interleaved_tx);
    memcpy(interleaved_rx, interleaved_tx, sizeof(interleaved_rx));

    for(i=0; i<sizeof(interleaved_rx); i++) {
        for(j=0; j<8; j++) {
            double x = (double)rand() / (double)RAND_MAX;
            if(x < ber) {
                (*nerrs)++;
                interleaved_rx[i] ^= (1<<j);
            }
        }
    }

    for(i=0; i<n_errors; i++) {
        int error_pos = rand() % (sizeof(interleaved_rx) * 8);
        (*nerrs)++;
        interleaved_rx[error_pos / 8] ^= (1<<(error_pos % 8));
    }

    interleaved_to_coded(interleaved_rx, coded_rx);
    coded_to_data(coded_rx, data_rx);

    for(i=0; i<sizeof(data_rx); i++) {
        if(data_rx[i] != data_tx[i]) {
            return false;
        }
    }
    
    return true;
}

double run_trials(double ber, unsigned int n_errors) {
    int i;
    int successes = 0;
    size_t n_errs = 0;
    const int num_trials = 500000;
    for(i=0; i<num_trials; i++) {
        if(run_trial(ber, n_errors, &n_errs)) {
            successes++;
        }
    }
    printf("avg num errs: %.04f\n", (double)n_errs / num_trials);
    /*printf("%d successes out of %d trials\n", successes, num_trials);*/
    return 1.0 - ((double)successes / (double)num_trials);
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    int i;

    srand(0);

    printf("\n#err\tBER\tPerr\n");
    for(i=0; i<21; i++) {
        double perr = run_trials(-1.0f, i);
        double ber = (double)i / 224.0f;
        printf("%d\t%.04f\t%.04f\n", i, ber, perr);
    }


    printf("\nBER\t#err\tPerr\n");
    for(i=0; i<210; i++) {
        double ber = (double)i / 1000.0f;
        double perr = run_trials(ber, 0);
        printf("%.03f\t%.04f\n", ber, perr);
    }

    return 0;
}
