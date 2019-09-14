import sys
from time import sleep
import matplotlib.pyplot as plt

sys.path.append('./pyhackrf')
from libhackrf import *

MIN_FREQ_MHZ = 50
# MAX_FREQ_MHZ = 2550
MAX_FREQ_MHZ = 150
SAMPLE_RATE_MHZ = 20

FFT_SIZE = 64

rx = HackRF(device_index = 0)
rx.sample_rate = SAMPLE_RATE_MHZ * 1e6
rx.close()

for rxf in range(MIN_FREQ_MHZ, MAX_FREQ_MHZ + 1, SAMPLE_RATE_MHZ):
    print("Fc = %s" % rxf)
    raw_input("Press Enter to start...")

    rx.open()
    rx.center_freq = rxf * 1e6
    rx.enable_amp()
    rx.lna_gain = 16
    rx.vga_gain = 22
    rx.receive_to_buffer()
    raw_input("Press Enter to stop...")

    samples = rx.stop_rx()
    mean = np.mean(samples)
    samples = samples - mean;

    print(len(samples))
    rx.close()

    Ps, fs = plt.psd(samples, NFFT=FFT_SIZE, Fs=rx.sample_rate/1e6, Fc=rx.center_freq/1e6)

    for i in range(len(Ps)):
        freq_to_power[fs[i]] = np.log(Ps[i])

with open('out-sweep.csv', 'w') as out:
    for (f,p) in freq_to_power.items():
        out.write('%s,%s\n' % (f, p))
