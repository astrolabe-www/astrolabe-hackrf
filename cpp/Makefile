CXXFLAGS ?= -O3 -march=native -fomit-frame-pointer -Wall
LDLIBS = -lhackrf -lfftw3f

INCDIR ?= /usr/local/include/libhackrf
BREWLIBDIR ?= /usr/local/lib

all: \
    bin/tx-rx \
    bin/test-rx \
    bin/test-tx \
    bin/nostop-rx \
    bin/nostop-tx \
    bin/nostop

bin/tx-rx: tx-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) tx-rx.c -o bin/tx-rx $(LDLIBS) $(LDFLAGS)
bin/test-rx: test-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) test-rx.c -o bin/test-rx $(LDLIBS) $(LDFLAGS)
bin/test-tx: test-tx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) test-tx.c -o bin/test-tx $(LDLIBS) $(LDFLAGS)
bin/nostop-rx: nostop-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) nostop-rx.c -o bin/nostop-rx $(LDLIBS) $(LDFLAGS)
bin/nostop-tx: nostop-tx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) nostop-tx.c -o bin/nostop-tx $(LDLIBS) $(LDFLAGS)
bin/nostop: nostop-tx-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) nostop-tx-rx.c -o bin/nostop $(LDLIBS) $(LDFLAGS)

clean:
	rm -f bin/*
