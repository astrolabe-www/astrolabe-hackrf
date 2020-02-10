#include <hackrf.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static hackrf_device* tx_device = NULL;

unsigned int MIN_FREQ_MHZ = 700;
unsigned int MAX_FREQ_MHZ = 800;
unsigned int SAMPLE_RATE_MHZ = 20;

// Last HackRf to be plugged in is id=0
int RX_ID = 0;

int tx_callback(hackrf_transfer* transfer) {
    for(unsigned int i = 0; i < transfer->valid_length; i++)
        transfer->buffer[i] = 0x7f;
    return 0;
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
    uint32_t txvga_gain = 42;
    uint32_t sample_rate_hz = SAMPLE_RATE_MHZ * 1e6;
    uint32_t txrx_freq_hz = MIN_FREQ_MHZ * 1e6;

    /////// init hackrf env
    if(check(hackrf_init(), "hackrf_init")) {
        return EXIT_FAILURE;
    }

    /////// init hackrf
    hackrf_device_list_t* device_list = hackrf_device_list();

    if(check(hackrf_device_list_open(device_list, (1 - RX_ID), &tx_device), "tx open")) {
        return EXIT_FAILURE;
    }


    /////// set sample rates
    if(check(hackrf_set_sample_rate(tx_device, sample_rate_hz), "tx sample rate")) {
        return EXIT_FAILURE;
    }
    
    if(check(hackrf_set_hw_sync_mode(tx_device, 0), "tx hw sync")){
        return EXIT_FAILURE;
    }

    ///// set tx gains
    if(check(hackrf_set_amp_enable(tx_device, 1) |
             hackrf_set_antenna_enable(tx_device, 0), "tx antenna + amp")) {
        return EXIT_FAILURE;
    }
    if(check(hackrf_set_txvga_gain(tx_device, txvga_gain), "tx gains")) {
        return EXIT_FAILURE;
    }


    ////// start and stop tx many times
    for(uint16_t rxf = MIN_FREQ_MHZ; rxf <= MAX_FREQ_MHZ; rxf += SAMPLE_RATE_MHZ) {
        fprintf(stdout, "txing %d MHz\n", rxf);
        //txrx_freq_hz = rxf * 1e6;
        txrx_freq_hz = 200 * 1e6;

        // start tx
        if(check(hackrf_set_freq(tx_device, txrx_freq_hz) |
                 hackrf_start_tx(tx_device, tx_callback, NULL), "tx start")) {
            return EXIT_FAILURE;
        }
        millisleep(1500);

        if(check(hackrf_stop_tx(tx_device), "tx stop")) {
            return EXIT_FAILURE;
        }
        millisleep(100);
    }


    //// close devices
    if(check(hackrf_close(tx_device), "tx close")) {
        return EXIT_FAILURE;
    }

    hackrf_exit();
    return EXIT_SUCCESS;
}
