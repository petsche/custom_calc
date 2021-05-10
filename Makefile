CC=gcc
CFLAGS=-I./src

all: build_dir run_tests

build_dir:
	mkdir -p build

build/int_impl.o: src/int/int_impl.c src/custom_calc.h
	$(CC) -c -o $@ $< $(CFLAGS)

build/custom_calc_int.o: src/custom_calc.c src/custom_calc.h
	$(CC) -c -o $@ $< $(CFLAGS) -DNUMBER_TYPE=int

build/test_calc_int: test/main_int.c build/int_impl.o build/custom_calc_int.o
	$(CC) -o $@ $^ $(CFLAGS) -DNUMBER_TYPE=int

run_tests: build/test_calc_int
	./$^
