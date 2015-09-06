all:
	mkdir -p build
	$(CC) -g --std=c99 -O3 -Wall -Wextra -Wpedantic -Werror -c -fPIC -o build/parity_matrix.o parity_matrix.c
	$(CC) -g --std=c99 -O3 -Wall -Wextra -Wpedantic -Werror -c -fPIC -o build/ldpc_decoder.o ldpc_decoder.c
	$(CC) -g --std=c99 -O3 -Wall -Wextra -Wpedantic -shared -fPIC -Wl,-soname,libldpc.so -o build/libldpc.so build/parity_matrix.o build/ldpc_decoder.o -lm
	$(CC) -g --std=c99 -O3 test.c -o build/test -Lbuild -lldpc

clean:
	rm *.o *.so
