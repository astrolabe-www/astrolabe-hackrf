#!/usr/bin/env python

import sys
import getopt

from os import path, listdir
from re import search, sub
from time import sleep

from pylab import *

def bytes2iq(data):
    values = np.array(data).astype(np.int8)
    iq = values.astype(np.float64).view(np.complex128)
    iq /= 127.5
    iq -= (1 + 1j)

    return iq

def main(argv):
    outfilename = 'pyout.csv'
    infileprefix = 'out_big'

    FFT_SIZE = 64

    options, remainder = getopt.getopt(argv, 'c:s:i:o:', ['fc=', 'fs=', 'id=', 'out='])
    for opt, arg in options:
        if opt in ('-c', '--fc'):
            fc = int(arg)
        elif opt in ('-s', '--fs'):
            fs = int(arg)
        elif opt in ('-i', '--id'):
            device_index = int(arg)
        elif opt in ('-o', '--out'):
            outfilename = str(arg)

    for filename in sort(listdir("out/")):
        if filename.startswith(infileprefix):
            Fc = int(sub('[\D]', '', filename))
            with open(path.join("out", filename), "rb") as file:
                samples = bytes2iq(bytearray(file.read()))
                mean = np.mean(samples)
                samples = samples - mean
                
                freq_to_power = {}
                Ps, fs = psd(samples, NFFT=FFT_SIZE, Fs=20, Fc=Fc)

                for i in range(len(Ps)):
                    if (fs[i] % 0.5 == 0.0):
                        freq_to_power[fs[i]] = np.log(Ps[i])

                with open(path.join('out', outfilename), 'a') as out:
                    for freq in freq_to_power:
                        out.write('%s,%s\n' % (freq, freq_to_power[freq]))

if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit()
