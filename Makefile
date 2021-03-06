# Makefile for Project #3

# Andrew Acosta (aacost06@calpoly.edu)
# Jason Swanson (jswans04@calpoly.edu)

CC= icpc
CFLAGS= -O3 -g -openmp -xHost
LIBS=
SRCFILES= AirParse.cpp tsp.cpp
INCLUDES=
OBJ= AirParse.o tsp.o

ifdef REPORT
CFLAGS+=-qopt-report=5
endif

all: partsp

partsp: $(SRCFILES) main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o partsp $^

clean:
	rm -f *.o partsp result.csv *.optrpt *~

testrun:
	./partsp test_routes.dat test_airports.dat

run:
	./partsp ../routes.dat ../airports.dat
