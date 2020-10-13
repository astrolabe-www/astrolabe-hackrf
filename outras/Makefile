CXXFLAGS ?= -O3 -march=native -fomit-frame-pointer -Wall
LDLIBS = -lhackrf

INCDIR ?= /usr/local/include/libhackrf
BREWLIBDIR ?= /usr/local/lib

all: bin/outras

bin/outras: outras.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) outras.c -o bin/outras $(LDLIBS) $(LDFLAGS)

clean:
	rm -f bin/*
