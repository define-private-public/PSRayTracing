# Note that this isn't really a "good" example of a Makefile; it's meant to be
# something that's quick and dirty to get the job done.  It works for GCC and
# clang.  You may need to modify it for a different compiler version or
# location.
#
# MSVC is a bit wonky, but kinda works.  You need to use some different
# (specific) commands/tagets and have the developer tools command line running.

GCC_CXX=g++-14
CLANG_CXX=clang++-18
MSVC_CXX=cl

SRC = comparing_greedy_vs_analytical.cpp

# Building the tests
all: build_gcc_14 build_clang_18

clean:
	rm test_*

build_gcc_14: $(SRC)
	$(GCC_CXX) $(SRC) -O0    -o test_gcc_14_O0
	$(GCC_CXX) $(SRC) -O1    -o test_gcc_14_O1
	$(GCC_CXX) $(SRC) -O2    -o test_gcc_14_O2
	$(GCC_CXX) $(SRC) -O3    -o test_gcc_14_O3
	$(GCC_CXX) $(SRC) -Ofast -o test_gcc_14_Ofast

build_clang_18: $(SRC)
	$(CLANG_CXX) $(SRC) -O0    -o test_clang_18_O0
	$(CLANG_CXX) $(SRC) -O1    -o test_clang_18_O1
	$(CLANG_CXX) $(SRC) -O2    -o test_clang_18_O2
	$(CLANG_CXX) $(SRC) -O3    -o test_clang_18_O3
	$(CLANG_CXX) $(SRC) -Ofast -o test_clang_18_Ofast

build_msvc: $(SRC)
	$(MSVC_CXX) $(SRC) /Od /Fetest_msvc_Od.exe
	$(MSVC_CXX) $(SRC) /Ot /Fetest_msvc_Ot.exe
	$(MSVC_CXX) $(SRC) /O2 /Fetest_msvc_O2.exe
	$(MSVC_CXX) $(SRC) /Ox /Fetest_msvc_Ox.exe

# Running the tests
RNG_SEED=1337
NUM_ITERATIONS=500
NUM_POINTS=1000000

run_all_tests:
	./test_gcc_14_O0 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_gcc_14_O1 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_gcc_14_O2 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_gcc_14_O3 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_gcc_14_Ofast $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_clang_18_O0 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_clang_18_O1 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_clang_18_O2 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_clang_18_O3 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
	./test_clang_18_Ofast $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
#	./test_msvc_Od $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
#	./test_msvc_Ot $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
#	./test_msvc_O2 $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
#	./test_msvc_Ox $(RNG_SEED) $(NUM_ITERATIONS) $(NUM_POINTS)
