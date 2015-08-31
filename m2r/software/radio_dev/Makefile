all:
	gcc -O3 -Wall -Wextra -Werror hamming_test.c hamming_ecc.c -o hamming
	gcc -g -march=native -O3 -fno-omit-frame-pointer -Wall -Wextra -Werror ldpc_test.c ldpc_encoder.c ldpc_decoder.c ldpc_parity_check.c ldpc_parity_check_packed.c -lm -o ldpc
	gcc -g -march=native -O3 -Wall -Wextra -Werror -c -fpic ldpc_decoder.c
	gcc -g -march=native -O3 -Wall -Wextra -Werror -c -fpic ldpc_parity_check.c
	gcc -g -march=native -shared -o libcldpcd.so ldpc_decoder.o ldpc_parity_check.o -lm
