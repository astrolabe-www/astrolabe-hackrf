import sys
from time import sleep
from subprocess import Popen

sys.path.append('./pyhackrf')
from libhackrf import *
from pylab import *

MIN_FREQ_MHZ = 50
MAX_FREQ_MHZ = 2550
SAMPLE_RATE_MHZ = 20

# Last HackRf to be plugged in is id=0
RX_ID = 0

tx = HackRF(device_index = (1 - RX_ID))
tx.sample_rate = SAMPLE_RATE_MHZ * 1e6
tx.close()

name = raw_input("Enter a name/nickname: ")
filename = "out-%s.csv" % name

for rxf in range(MIN_FREQ_MHZ, MAX_FREQ_MHZ + 1, SAMPLE_RATE_MHZ):
    print('Fc = %s' % rxf)
    tx.open()
    tx.center_freq = rxf * 1e6
    tx.enable_amp()
    tx.txvga_gain = 42
    tx.transmit_noise()
    rxp = Popen(['./rx-cmd.py', '-c%s'%rxf, '-s%s'%SAMPLE_RATE_MHZ, '-i%s'%RX_ID, '-o%s'%filename])
    rxp.wait()
    tx.stop_tx()
    tx.close()
    sleep(0.1)

sleep(0.5)
