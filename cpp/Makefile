CXXFLAGS ?= -O3 -march=native -fomit-frame-pointer -Wall
LDLIBS = -lhackrf -lfftw3f

INCDIR ?= /usr/local/include/libhackrf
BREWLIBDIR ?= /usr/local/lib

all: tx-rx test-rx test-tx nostop-rx nostop-tx

tx-rx: tx-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) tx-rx.c -o tx-rx $(LDLIBS) $(LDFLAGS)
test-rx: test-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) test-rx.c -o test-rx $(LDLIBS) $(LDFLAGS)
test-tx: test-tx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) test-tx.c -o test-tx $(LDLIBS) $(LDFLAGS)
nostop-rx: nostop-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) nostop-rx.c -o nostop-rx $(LDLIBS) $(LDFLAGS)
nostop-tx: nostop-tx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) nostop-tx.c -o nostop-tx $(LDLIBS) $(LDFLAGS)

clean:
	rm -f tx-rx test-rx test-tx nostop-rx nostop-tx
