#Makefile
#!/bin/sh

CC=mpicc
CXX=mpicxx
CXXFLAGS=-O3 
ONECCL_ROOT=/home/hzheng/soft/datascience/oneCCL
allreduce:
	$(CXX) $(CXXFLAGS) -o allreduce.x allreduce.cpp $(HPCTW)

ccl_allreduce_bench:
	$(CXX) $(CXXFLAGS) -I$(ONECCL_ROOT)/include -Wall -Werror -D_GNU_SOURCE -fvisibility=internal -O3 -DNDEBUG  -O3   -o ccl_allreduce_bench.c.o -c ccl_allreduce_bench.c
	$(CXX) $(CXXFLAGS) -Wall -Werror -D_GNU_SOURCE -fvisibility=internal -O3 -DNDEBUG  -O3 -o ccl_allreduce_bench ccl_allreduce_bench.c.o -rdynamic -lrt -lm -L$(ONECCL_ROOT)/lib/ -lccl -ldl -lpthread
