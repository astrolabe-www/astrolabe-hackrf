import sys
from time import sleep

sys.path.append('./pyhackrf')
from libhackrf import *
from pylab import *

hackrf = HackRF()

hackrf.sample_rate = 20e6
hackrf.center_freq = 92e6

hackrf.enable_amp()
hackrf.txvga_gain = 39

hackrf.transmit_noise()
sleep(.05)
hackrf.stop_tx()
