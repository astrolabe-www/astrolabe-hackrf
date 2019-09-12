import sys
from time import sleep
from subprocess import Popen

sys.path.append('./pyhackrf')
from libhackrf import *
from pylab import *

MIN_FREQ_MHZ = 50
# MAX_FREQ_MHZ = 2550
MAX_FREQ_MHZ = 150
SAMPLE_RATE_MHZ = 20

tx = HackRF(device_index = 1, is_tx=True)
tx.sample_rate = SAMPLE_RATE_MHZ * 1e6
tx.close()

for rxf in range(MIN_FREQ_MHZ, MAX_FREQ_MHZ + 1, SAMPLE_RATE_MHZ+200):
    print(rxf)
    rxp = Popen(['./rx.py', '-c%s'%rxf, '-s%s'%SAMPLE_RATE_MHZ, '-i%s'%0, '-o%s'%rxf])
    sleep(.4)

    for txf in range(rxf - SAMPLE_RATE_MHZ/2, rxf + SAMPLE_RATE_MHZ/2, 2):
        print("\t" + str(txf))
        tx.open()
        tx.center_freq = txf * 1e6
        tx.transmit_noise()
        sleep(.1)
        tx.stop_tx()
        tx.close()

    rxp.wait()
