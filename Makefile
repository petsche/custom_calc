CC=gcc
CFLAGS=-I./src -I./test

VARIANTS=int double

DEFINES_FOR_int=-DCALC_NUMBER_TYPE=int -DCALC_IO_WIDTH=8
DEFINES_FOR_double=-DCALC_NUMBER_TYPE=double -DCALC_IO_WIDTH=16

define make_calc_variant =
build/custom_calc_$(1).o: src/custom_calc.c src/custom_calc.h
	$$(CC) $$(CFLAGS) $$(DEFINES_FOR_$(1)) -c $$< -o $$@

build/user_$(1).o: src/user/user_$(1).c src/custom_calc.h
	$$(CC) $$(CFLAGS) $$(DEFINES_FOR_$(1)) -c $$< -o $$@

build/test_util_$(1).o: test/test_util.c test/test_util.h src/custom_calc.h
	$$(CC) $$(CFLAGS) $$(DEFINES_FOR_$(1)) -c $$< -o $$@

build/test_$(1): test/test_$(1).c build/user_$(1).o build/custom_calc_$(1).o build/test_util_$(1).o
	$$(CC) $$(CFLAGS) $$(DEFINES_FOR_$(1)) $$^ -o $$@ 

endef

all: build_dir build/infix_int run_tests 

build_dir:
	mkdir -p build

$(eval $(foreach variant,$(VARIANTS),$(call make_calc_variant,$(variant))))

build/infix_int: test/infix_int.c build/user_int.o build/custom_calc_int.o
	$(CC) -o $@ $^ $(CFLAGS) -DCALC_NUMBER_TYPE=int

run_tests: build/test_int build/test_double
	./build/test_int && ./build/test_double
