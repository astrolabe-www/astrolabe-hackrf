#!/usr/bin/env python

import sys

from os import path, listdir, getenv
from re import findall, sub
from time import sleep

from requests import post

from dotenv import load_dotenv
load_dotenv()

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
    for filename in sorted(listdir("tmp/")):
        if filename.startswith(infileprefix):
            powers = []
            Fc = int(sub(r'[_\.csv]', '', findall(r'_[0-9]+\.csv', filename)[0]))

            with open(path.join("tmp", filename), "rb") as file:
                samples = bytes2iq(bytearray(file.read()))
                mean = np.mean(samples)
                samples = samples - mean

                Ps, fs = plt.psd(samples, NFFT=FFT_SIZE, Fs=20, Fc=Fc)

                for i in range(len(Ps)):
                    powers.append(np.log(Ps[i]))

            avg = sum(powers) / len(powers)
            freq_to_avg[Fc] = avg

    for (k, v) in freq_to_avg.iteritems():
      vnorm = (v + 10.0) / 10.0
      url = "https://%s%s%s/HACKRF_%s/%0.2f" % (getenv('API_URL'),
                                                getenv('API_ENDPOINT'),
                                                getenv('API_TOKEN'),
                                                k, vnorm)
      print url
      post(url)

if __name__ == "__main__":
    main(sys.argv[1:])
    sys.exit()
