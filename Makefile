# Makefile for Project #3

# Andrew Acosta (aacost06@calpoly.edu)
# Jason Swanson (jswans04@calpoly.edu)

CC= icpc
CFLAGS= -O3 -g #-debug inline-debug-info -openmp -xHost
LIBS=
SRCFILES= AirParse.cpp airport.cpp route.cpp main.cpp tsp.cpp
PARSETESTSRCFILES= AirParse.cpp tsp.cpp
INCLUDES=
OBJ= AirParse.o tsp.o

ifdef REPORT
CFLAGS+=-qopt-report=5
endif

all: partsp

mmomp: $(SRCFILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o partsp $^

testparse: $(OBJ) testMain.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -o testparse $^

%.o: $.c $(PARSETESTSRCFILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
	

clean:
	rm -f *.o partsp testparse result.csv *.optrpt *~
