all:
	@echo 'Try `make test`'


test:
	gcc -Wall -Wextra -Werror m2telem.c m2telem_test.c -o m2telem_test
	./m2telem_test

dump:
	gcc -Wall -Wextra -Werror -O3 m2telem.c m2telem_dump.c -o m2telem_dump
