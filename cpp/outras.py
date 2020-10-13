#!/usr/bin/env python

import sys

from os import path, listdir
from re import findall, sub
from time import sleep

import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt
import numpy as np

def bytes2iq(data):
    values = np.array(data).astype(np.int8)
    iq = values.astype(np.float64).view(np.complex128)
    iq /= 127.5
    iq -= (1 + 1j)
    return iq

def main(argv):
    infileprefix = "out_outras"
    FFT_SIZE = 64

    ## TODO:
    ## once a minute:
      ## clear out/
      ## run outras.c

    freq_to_avg = {}
    for filename in sorted(listdir("out/")):
        if filename.startswith(infileprefix):
            powers = []
            Fc = int(sub(r'[_\.csv]', '', findall(r'_[0-9]+\.csv', filename)[0]))
            print(Fc)
            with open(path.join("out", filename), "rb") as file:
                samples = bytes2iq(bytearray(file.read()))
                mean = np.mean(samples)
                samples = samples - mean

                Ps, fs = plt.psd(samples, NFFT=FFT_SIZE, Fs=20, Fc=Fc)

                for i in range(len(Ps)):
                    powers.append(np.log(Ps[i]))

            avg = sum(powers) / len(powers)
            freq_to_avg[Fc] = avg
            print "%s %s" % (Fc, avg)

    ## TODO:
    ## map avgs: [-10, 0] -> [0, 1]
    ## POST to API

if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit()
