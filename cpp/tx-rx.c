#include <hackrf.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static hackrf_device* tx_device = NULL;
static hackrf_device* rx_device = NULL;

unsigned int MIN_FREQ_MHZ = 50;
// int MAX_FREQ_MHZ = 2550;
unsigned int MAX_FREQ_MHZ = 150;
unsigned int SAMPLE_RATE_MHZ = 20;

// Last HackRf to be plugged in is id=0
int RX_ID = 0;

uint64_t samples_to_rxfer = 2e6;
uint64_t bytes_to_rxfer = samples_to_rxfer * 2;

int tx_callback(hackrf_transfer* transfer) {
    for(unsigned int i = 0; i < transfer->buffer_length; i++)
        transfer->buffer[i] = (uint8_t)(rand() & 0xff);
    return 0;
}

int rx_callback(hackrf_transfer* transfer) {
	int bytes_to_read = transfer->valid_length;

    if (bytes_to_read >= bytes_to_rxfer) {
        bytes_to_read = bytes_to_rxfer;
    }
    bytes_to_rxfer -= bytes_to_read;

    // TODO: send this to a proper place
    for(unsigned int i = 0; i < bytes_to_read; i++)
        transfer->buffer[i] = transfer->buffer[i];

    return -1 * (bytes_to_rxfer == 0);
}

void millisleep(int mls) {
    struct timespec sleep_m;
    sleep_m.tv_sec = mls / 1000;
    sleep_m.tv_nsec = (mls % 1000) * 1000 * 1000;
    nanosleep(&sleep_m, NULL);
    return;
}

int check(int result, const char msg[]) {
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "%s failed: %s (%d)\n",
                msg,
                hackrf_error_name((hackrf_error)result),
                result);
        hackrf_exit();
        return HACKRF_ERROR_OTHER;
    } else {
        return HACKRF_SUCCESS;
    }
}

int main(int argc, char** argv) {
	unsigned int lna_gain=16, vga_gain=22, txvga_gain=42;
    uint32_t sample_rate_hz = SAMPLE_RATE_MHZ * 1e6;
    uint32_t txrx_freq_hz = MIN_FREQ_MHZ * 1e6;

    /////// init hackrf env
    if(check(hackrf_init(), "hackrf_init")) {
        return EXIT_FAILURE;
    }

    /////// init 2 hackrfs
    hackrf_device_list_t* device_list = hackrf_device_list();

    if(check(hackrf_device_list_open(device_list, (1 - RX_ID), &tx_device), "tx open")) {
        return EXIT_FAILURE;
    }

    if(check(hackrf_device_list_open(device_list, RX_ID, &rx_device), "rx open")) {
        return EXIT_FAILURE;
    }

    /////// set sample rates
    if(check(hackrf_set_sample_rate(tx_device, sample_rate_hz), "tx sample rate")) {
        return EXIT_FAILURE;
    }
    if(check(hackrf_set_sample_rate(rx_device, sample_rate_hz), "rx sample rate")) {
        return EXIT_FAILURE;
    }

    ///// set tx gains
    if(check(hackrf_set_amp_enable(tx_device, 1), "tx amp")) {
        return EXIT_FAILURE;
    }
    if(check(hackrf_set_txvga_gain(tx_device, txvga_gain), "tx gains")) {
        return EXIT_FAILURE;
    }

    ///// set rx gains
    if(check(hackrf_set_amp_enable(rx_device, 1), "rx amp")) {
        return EXIT_FAILURE;
    }
    if(check(hackrf_set_vga_gain(rx_device, vga_gain) |
             hackrf_set_lna_gain(rx_device, lna_gain), "rx gains")) {
        return EXIT_FAILURE;
    }


    ////// start and stop tx many times
    for(unsigned int rxf = MIN_FREQ_MHZ; rxf <= MAX_FREQ_MHZ; rxf += SAMPLE_RATE_MHZ) {
        fprintf(stdout, "txing %d MHz\n", rxf);
        txrx_freq_hz = rxf * 1e6;

        // start tx
        if(check(hackrf_set_freq(tx_device, txrx_freq_hz) |
                 hackrf_start_tx(tx_device, tx_callback, NULL), "tx start")) {
            return EXIT_FAILURE;
        }


        // start rx
        bytes_to_rxfer = samples_to_rxfer * 2;
        if(check(hackrf_set_freq(rx_device, txrx_freq_hz) |
                 hackrf_start_rx(rx_device, rx_callback, NULL), "rx start")) {
            return EXIT_FAILURE;
        }

        while(bytes_to_rxfer > 0) {
            millisleep(10);
        }
        fprintf(stdout, "done rxing!! \n");

        // stop rx and tx
        if(check(hackrf_stop_rx(rx_device), "rx stop")) {
            return EXIT_FAILURE;
        }
        if(check(hackrf_stop_tx(tx_device), "tx stop")) {
            return EXIT_FAILURE;
        }

        /*
        TODO: 
        mean = np.mean(samples)
        samples = samples - mean;

        Ps, fs = psd(samples, NFFT=FFT_SIZE, Fs=rx.sample_rate/1e6, Fc=rx.center_freq/1e6)

        for i in range(len(Ps)):
            if (fs[i] % 1.0 == 0.0):
                freq_to_power[fs[i]] = np.log(Ps[i])

        with open(os.path.join('out', outfilename), 'a') as out:
            for freq in freq_to_power:
                out.write('%s,%s\n' % (freq, freq_to_power[freq]))
        */

        millisleep(100);
    }

    //// close devices
    if(check(hackrf_close(tx_device), "tx close")) {
        return EXIT_FAILURE;
    }
    if(check(hackrf_close(rx_device), "rx close")) {
        return EXIT_FAILURE;
    }

    hackrf_exit();
    return EXIT_SUCCESS;
}
