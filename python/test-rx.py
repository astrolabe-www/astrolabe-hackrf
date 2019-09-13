import sys
import numpy as np

sys.path.append('./pyhackrf')
from libhackrf import *
from pylab import *

hackrf = HackRF()

hackrf.sample_rate = 20e6
hackrf.center_freq = 92e6

samples = hackrf.read_samples(2e6)
mean = np.mean(samples)
samples = samples - mean;

Ps, fs = psd(samples, NFFT=128, Fs=hackrf.sample_rate/1e6, Fc=hackrf.center_freq/1e6)
xlabel('Frequency (MHz)')
ylabel('Relative power (dB)')
show()
