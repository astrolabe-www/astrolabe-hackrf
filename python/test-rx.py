import sys
sys.path.append('./pyhackrf')

from libhackrf import *
from pylab import *

hackrf = HackRF()

hackrf.sample_rate = 20e6
hackrf.center_freq = 1797e6

samples = hackrf.read_samples(2e6)

Ps, fs = psd(samples, NFFT=1024, Fs=hackrf.sample_rate/1e6, Fc=hackrf.center_freq/1e6)
xlabel('Frequency (MHz)')
ylabel('Relative power (dB)')
show()
