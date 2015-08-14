#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "ldpc_encoder.h"
#include "ldpc_decoder.h"

double randn()
{
    double u1 = (double)rand() / (double)RAND_MAX;
    double u2 = (double)rand() / (double)RAND_MAX;
    return sqrt(-2.0f * log(u1)) * cos(2 * M_PI * u2);
}

bool run_trial(double snr)
{
    uint8_t data_tx[16], coded_tx[32], coded_rx[32], data_rx[16];
    double llrs[256];
    double sigma = sqrt(1.0f / snr);
    int i;
    bool decode_valid = false;

    memset(data_rx, 0, sizeof(data_rx));
    memset(coded_tx, 0, sizeof(coded_tx));
    memset(coded_rx, 0, sizeof(coded_rx));
    memset(llrs, 0, sizeof(llrs));

    for(i=0; i<16; i++) data_tx[i] = rand() & 0xFF;

    ldpc_encode(data_tx, coded_tx);

    for(i=0; i<256; i++) {
        uint8_t coded_byte = i / 8;
        uint8_t coded_bit = 7 - (i % 8);
        uint8_t bit = (coded_tx[coded_byte] >> coded_bit) & 1;
        double x, y, p_y_xp, p_y_xm, p_x1_y, p_x0_y;

        if(bit)
            x = 1.0;
        else
            x = -1.0;

        y = x + randn() * sigma;
        p_y_xp = 1.0f/sqrt(2.0f*M_PI*sigma*sigma) *
                 exp( -(y - 1)*(y - 1) / (2*sigma*sigma));
        p_y_xm = 1.0f/sqrt(2.0f*M_PI*sigma*sigma) *
                 exp( -(y + 1)*(y + 1) / (2*sigma*sigma));
        p_x1_y = p_y_xp / (p_y_xp + p_y_xm);
        p_x0_y = 1.0f - p_x1_y;
        llrs[i] = log(p_x0_y / p_x1_y);
        /*printf("%.04f ", llrs[i]);*/
    }

    ldpc_decode(llrs, coded_rx);
    memcpy(data_rx, coded_rx, 16);

    /*
    printf("\nTX Data:\n");
    for(i=0; i<16; i++) printf("%02X ", data_tx[i]);

    printf("\nTX Codeword:\n");
    for(i=0; i<32; i++) printf("%02X ", coded_tx[i]);

    printf("\nRX Codeword:\n");
    for(i=0; i<32; i++) printf("%02X ", coded_rx[i]);

    printf("\nRX Data:\n");
    for(i=0; i<16; i++) printf("%02X ", data_rx[i]);
    printf("\n");
    */

    decode_valid = true;
    for(i=0; i<16; i++) {
        if(data_rx[i] != data_tx[i]) {
            decode_valid = false;
            break;
        }
    }

    return decode_valid;
}

double run_trials(double snr)
{
    const int n_trials = 500;
    int i;
    int successes = 0;
    for(i=0; i<n_trials; i++) {
        if(run_trial(snr))
            successes++;
    }
    return (double)successes / (double)n_trials;
}

int main(int argc, char* argv[])
{
    int i;
    (void)argc; (void)argv;
    srand(time(NULL));
    printf("SNR (dB)\tPacket Err Rate\n");
    for(i=0; i<200; i+=20) {
        double snr_db = (double)i/10.0f;
        double snr = pow(10.0f, snr_db/10.0f);
        double per = 1.0f - run_trials(snr);
        printf("%.1f\t\t%.4e\n", snr_db, per);
    }

    return 0;
}
