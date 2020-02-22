#Makefile
#!/bin/sh

CC=mpicc
CXX=mpicxx
CXXFLAGS=-O3 
allreduce:
	$(CXX) $(CXXFLAGS) -o allreduce.x allreduce.cpp $(HPCTW)
