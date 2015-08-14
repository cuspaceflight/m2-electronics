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
    double tx_pwr = 0.0f, noise_pwr = 0.0f;
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
        double x, n, y, p_y_xp, p_y_xm, p_x1_y, p_x0_y;
        uint8_t hard_decision;
        char hard_correct;

        if(bit)
            x = 1.0;
        else
            x = -1.0;

        n = randn() * sigma;
        tx_pwr += x*x;
        noise_pwr += n*n;
        y = x + n;
        p_y_xp = exp( -(y - 1)*(y - 1) / (2*sigma*sigma));
        p_y_xm = exp( -(y + 1)*(y + 1) / (2*sigma*sigma));
        p_x1_y = p_y_xp / (p_y_xp + p_y_xm);
        p_x0_y = 1.0f - p_x1_y;
        hard_decision = p_x1_y > p_x0_y;
        hard_correct = hard_decision == bit ? 'y' : 'n';
        (void)hard_correct;
        /*printf("x=%+.0f\ty=%+.4f\tP(y|x=+)=%.4f\t P(y|x=-)=%.4f\t P(x=1|y)=%.4f\t P(x=0|y)=%.4f\t(%d: %c)\n", x, y, p_y_xp, p_y_xm, p_x1_y, p_x0_y, hard_decision, hard_correct);*/
        llrs[i] = log(p_x0_y / p_x1_y);
        /*printf("%.04f ", llrs[i]);*/
    }

    tx_pwr /= 256.0f;
    noise_pwr /= 256.0f;
    (void)tx_pwr;
    (void)noise_pwr;
    /*printf("TX Power: %.2f\nNoise Power: %.2f\n", tx_pwr, noise_pwr);*/
    /*printf("Nominal SNR: %.2f (%.2fdB)\n", snr, 10.0f*log10(snr));*/
    /*printf("Sigma: %.2f\n", sigma);*/
    /*printf("Empirical SNR: %.2f (%.2fdB)\n", tx_pwr / noise_pwr, 10.0f*log10(tx_pwr / noise_pwr));*/

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
    const int max_trials = 1000;
    int n_trials, n_errors = 0;
    for(n_trials=0; n_errors < 1000 && n_trials<max_trials; n_trials++) {
        if(!run_trial(snr))
            n_errors++;
    }
    return (double)n_errors / (double)n_trials;
}

int main(int argc, char* argv[])
{
    int i;
    (void)argc; (void)argv;
    srand(time(NULL));
    srand(0);

    printf("SNR (dB)\tPacket Err Rate\n");
    for(i=30; i<=30; i+=5) {
        double snr_db = (double)i/10.0f;
        double snr = pow(10.0f, snr_db/10.0f);
        double per = run_trials(snr);
        printf("%.1f\t\t%.4e\n", snr_db, per);
    }

    return 0;
}
