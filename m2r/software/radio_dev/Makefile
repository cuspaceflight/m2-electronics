all:
	gcc -O3 -Wall -Wextra -Werror hamming_test.c hamming_ecc.c -o hamming
	gcc -g -O3 -fno-omit-frame-pointer -Wall -Wextra -Werror ldpc_test.c ldpc_encoder.c ldpc_decoder.c ldpc_parity_check.c ldpc_parity_check_packed.c -lm -o ldpc
