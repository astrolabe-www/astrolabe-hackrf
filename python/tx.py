import sys
from time import sleep

sys.path.append('./pyhackrf')
from libhackrf import *

MIN_FREQ_MHZ = 50
# MAX_FREQ_MHZ = 2550
MAX_FREQ_MHZ = 150
SAMPLE_RATE_MHZ = 20

tx = HackRF(device_index = 0)
tx.sample_rate = SAMPLE_RATE_MHZ * 1e6
tx.close()

for rxf in range(MIN_FREQ_MHZ, MAX_FREQ_MHZ + 1, SAMPLE_RATE_MHZ):
    print("Fc = %s" % rxf)
    raw_input("Press Enter to continue...")

    for txf in range(rxf - SAMPLE_RATE_MHZ/2, rxf + SAMPLE_RATE_MHZ/2, 2):
        print("\t" + str(txf))
        tx.open()
        tx.center_freq = txf * 1e6
        tx.enable_amp()
        tx.txvga_gain = 39
        tx.transmit_noise()
        sleep(.1)
        tx.stop_tx()
        tx.close()
