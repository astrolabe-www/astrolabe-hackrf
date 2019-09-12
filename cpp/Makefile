CXXFLAGS ?= -O3 -march=native -fomit-frame-pointer -Wall
LDLIBS = -lhackrf

# PREFIX ?= /Applications/GNURadio.app/Contents/MacOS/usr
# LIBDIR ?= $(PREFIX)/lib
INCDIR ?= /usr/local/include/libhackrf
BREWLIBDIR ?= /usr/local/lib

all: tx-rx

tx-rx: tx-rx.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCDIR) -L$(BREWLIBDIR) tx-rx.c -o tx-rx $(LDLIBS) $(LDFLAGS)
clean:
	rm -f tx-rx
