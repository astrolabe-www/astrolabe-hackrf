CXXFLAGS ?= -O3 -march=native -fomit-frame-pointer -Wall
LDLIBS = -lhackrf -lfftw3f

INCDIR ?= /usr/local/include/libhackrf
BREWLIBDIR ?= /usr/local/lib

all: tx-rx rx tx

tx-rx: tx-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) tx-rx.c -o tx-rx $(LDLIBS) $(LDFLAGS)
rx: rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) rx.c -o rx $(LDLIBS) $(LDFLAGS)
tx: tx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) tx.c -o tx $(LDLIBS) $(LDFLAGS)

clean:
	rm -f tx-rx rx tx
