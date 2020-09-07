#!/usr/bin/env python

import sys

from os import path, listdir
from re import findall, sub
from time import sleep

import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt

def bytes2iq(data):
    values = np.array(data).astype(np.int8)
    iq = values.astype(np.float64).view(np.complex128)
    iq /= 127.5
    iq -= (1 + 1j)

    return iq

def main(argv):
    name = raw_input("Enter file prefix: ")

    infileprefix = "out_%s" % name
    outfilename = 'pyout_%s.csv' % name

    FFT_SIZE = 64

    freq_to_power = {}
    for filename in sort(listdir("out/")):
        if filename.startswith(infileprefix):
            Fc = int(sub(r'[_\.csv]', '', findall(r'_[0-9]+\.csv', filename)[0]))
            print(Fc)
            with open(path.join("out", filename), "rb") as file:
                samples = bytes2iq(bytearray(file.read()))
                mean = np.mean(samples)
                samples = samples - mean

                Ps, fs = plt.psd(samples, NFFT=FFT_SIZE, Fs=20, Fc=Fc)

                for i in range(len(Ps)):
                    if (fs[i] % 1.0 == 0.0):
                        freq_to_power[fs[i]] = np.log(Ps[i])

    with open(path.join('out', outfilename), 'a') as out:
        for freq in freq_to_power:
            out.write('%s,%s\n' % (freq, freq_to_power[freq]))

if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit()
