#!/usr/bin/env python

import sys
import os.path

import getopt
from time import sleep

sys.path.append('./pyhackrf')
from libhackrf import *
from pylab import *

def main(argv):
    device_index = 0
    fs = 20
    fc = 90
    outfilename = 'out.csv'

    FFT_SIZE = 64
    freq_to_power = {}

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

    rx = HackRF(device_index = device_index)
    rx.sample_rate = fs * 1e6
    rx.center_freq = fc * 1e6
    rx.enable_amp()
    rx.lna_gain = 16
    rx.vga_gain = 22

    samples = rx.read_samples(2e6)
    mean = np.mean(samples)
    samples = samples - mean;

    rx.close()

    Ps, fs = psd(samples, NFFT=FFT_SIZE, Fs=rx.sample_rate/1e6, Fc=rx.center_freq/1e6)

    for i in range(len(Ps)):
        freq_to_power[fs[i]] = np.log(Ps[i])

    with open(os.path.join('out', outfilename), 'a') as out:
        for i in range(len(Ps)):
            out.write('%s,%s\n' % (fs[i], freq_to_power[fs[i]]))

if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit()
