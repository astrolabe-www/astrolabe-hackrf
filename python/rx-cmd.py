#!/usr/bin/env python

import sys
import getopt
from time import sleep

sys.path.append('./pyhackrf')
from libhackrf import *
from pylab import *

def main(argv):
    device_index = 0
    fs = 20
    fc = 90
    outfilename = 'out'

    options, remainder = getopt.getopt(sys.argv[1:], 'c:s:i:o:', ['fc=', 'fs=', 'id=', 'out='])
    for opt, arg in options:
        if opt in ('-c', '--fc'):
            fc = int(arg)
        elif opt in ('-s', '--fs'):
            fs = int(arg)
        elif opt in ('-i', '--id'):
            device_index = int(arg)
        elif opt in ('-o', '--out'):
            outfilename = str(arg)

    rx = HackRF(device_index = device_index)
    rx.sample_rate = fs * 1e6
    rx.center_freq = fc * 1e6

    samples = rx.read_samples(10e6)
    rx.close()

    Ps, fs = psd(samples, NFFT=1024, Fs=rx.sample_rate/1e6, Fc=rx.center_freq/1e6)
    Ps[511] = Ps[512] = Ps[513] = Ps[514]

    with open('%s.csv' % outfilename, 'a') as out:
        for i in range(len(Ps)):
            out.write('%s,%s\n' % (fs[i], Ps[i]))

    sys.exit()

if __name__ == "__main__":
    main(sys.argv[1:])