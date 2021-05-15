CC=gcc
CFLAGS=-I./src

all: build_dir build/infix_int run_tests 

build_dir:
	mkdir -p build

build/user_int.o: src/user/user_int.c src/custom_calc.h
	$(CC) -c -o $@ $< $(CFLAGS) -DCALC_NUMBER_TYPE=int

build/custom_calc_int.o: src/custom_calc.c src/custom_calc.h
	$(CC) -c -o $@ $< $(CFLAGS) -DCALC_NUMBER_TYPE=int

build/test_int: test/test_int.c build/user_int.o build/custom_calc_int.o
	$(CC) -o $@ $^ $(CFLAGS) -DCALC_NUMBER_TYPE=int

build/infix_int: test/infix_int.c build/user_int.o build/custom_calc_int.o
	$(CC) -o $@ $^ $(CFLAGS) -DCALC_NUMBER_TYPE=int

run_tests: build/test_int
	./$^
