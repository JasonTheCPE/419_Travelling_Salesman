# Makefile for Project #3

# Andrew Acosta (aacost06@calpoly.edu)
# Jason Swanson (jswans04@calpoly.edu)

CC= icc
CFLAGS= -O3 -g -debug inline-debug-info -openmp -xHost
LIBS=
SRCFILES= AirParse.cpp airport.cpp route.cpp main.cpp
PARSETESTSRCFILES= AirParse.cpp airport.cpp route.cpp testMain.cpp
INCLUDES=

ifdef REPORT
CFLAGS+=-qopt-report=5
endif

all: partsp

mmomp: $(SRCFILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o partsp $^

testparse: $(PARSETESTSRCFILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o testparse $^

clean:
	rm -f *.o partsp result.csv *.optrpt
