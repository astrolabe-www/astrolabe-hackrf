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

uint64_t samples_to_txfer = 2e6;
uint64_t bytes_to_txfer = samples_to_txfer * 2;

uint64_t samples_to_rxfer = samples_to_txfer * 11;
uint64_t bytes_to_rxfer = samples_to_rxfer * 2;


int tx_callback(hackrf_transfer* transfer) {
	size_t bytes_to_read;
	unsigned int i;

    /* Transmit continuous wave with specific amplitude */
    bytes_to_read = transfer->valid_length;
    if (bytes_to_read >= bytes_to_txfer) {
        bytes_to_read = bytes_to_txfer;
    }
    bytes_to_txfer -= bytes_to_read;

    for(i = 0; i < bytes_to_read; i++)
        transfer->buffer[i] = 100;

    return -1 * (bytes_to_txfer == 0);
}

int rx_callback(hackrf_transfer* transfer) {
	size_t bytes_to_write;
	unsigned int i;

    bytes_to_write = transfer->valid_length;
    if (bytes_to_write >= bytes_to_rxfer) {
        bytes_to_write = bytes_to_rxfer;
    }
    bytes_to_rxfer -= bytes_to_write;

    // TODO: send this to a proper place
    for(i = 0; i < bytes_to_write; i++)
        transfer->buffer[i] = transfer->buffer[i];

    return -1 * (bytes_to_rxfer == 0);
}

int main(int argc, char** argv) {
	int result;
	int exit_code = EXIT_SUCCESS;
	unsigned int lna_gain=16, vga_gain=16, txvga_gain=39;
    const char* tx_serial_number = "0000000000000000909864c82d0a44cf";
    const char* rx_serial_number = "0000000000000000909864c8385238cf";
    uint32_t sample_rate_hz = SAMPLE_RATE_MHZ * 1e6;
    uint32_t tx_freq_hz = 92 * 1e6;
    uint32_t rx_freq_hz = 92 * 1e6;

    struct timespec sleep_100m;
    sleep_100m.tv_sec = 0;
    sleep_100m.tv_nsec = 100 * 1000;

    /////// init hackrf env
    result = hackrf_init();
	if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "hackrf_init() failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    /////// init 2 hackrfs
    result = hackrf_open_by_serial(tx_serial_number, &tx_device);
	if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "tx hackrf_open() failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    result = hackrf_open_by_serial(rx_serial_number, &rx_device);
	if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "rx hackrf_open() failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    /////// set sample rates
    result = hackrf_set_sample_rate(tx_device, sample_rate_hz);
	if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "tx hackrf_set_sample_rate() failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    result = hackrf_set_sample_rate(rx_device, sample_rate_hz);
	if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "rx hackrf_set_sample_rate() failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    ///// set tx gains
    result = hackrf_set_txvga_gain(tx_device, txvga_gain);
    if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "hackrf tx gain failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    ///// set rx gains
    result = hackrf_set_vga_gain(rx_device, vga_gain);
    result |= hackrf_set_lna_gain(rx_device, lna_gain);
    if( result != HACKRF_SUCCESS ) {
		fprintf(stderr, "hackrf rx gain failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
		return EXIT_FAILURE;
	}

    // start txs and rxs
    for(unsigned int rxf = MIN_FREQ_MHZ; rxf < MAX_FREQ_MHZ + 1; rxf += SAMPLE_RATE_MHZ) {
        fprintf(stdout, "%d\n", rxf);
        // start and stop rx at different frequencies
        rx_freq_hz = rxf * 1e6;

        bytes_to_rxfer = samples_to_rxfer * 2;
        result = hackrf_start_rx(rx_device, rx_callback, NULL);
        result = hackrf_set_freq(rx_device, rx_freq_hz);
        if( result != HACKRF_SUCCESS ) {
            fprintf(stderr, "hackrf rx in loop failed: %s (%d)\n",
                    hackrf_error_name((enum hackrf_error)result),
                    result);
            return EXIT_FAILURE;
        }

        for(unsigned int txf = rxf - SAMPLE_RATE_MHZ/2; txf < rxf + SAMPLE_RATE_MHZ/2; txf += 2) {
            // start and stop tx at different frequencies
            fprintf(stdout, "\t%d\n", txf);
            tx_freq_hz = txf * 1e6;

            bytes_to_txfer = samples_to_txfer * 2;
            result = hackrf_start_tx(tx_device, tx_callback, NULL);
            result |= hackrf_set_freq(tx_device, tx_freq_hz);
            
            if( result != HACKRF_SUCCESS ) {
            fprintf(stderr, "hackrf tx in loop failed: %s (%d)\n",
                    hackrf_error_name((enum hackrf_error)result),
                    result);
            return EXIT_FAILURE;
            }

            // fprintf(stdout, "sleep\n");
            while(hackrf_is_streaming(tx_device) == HACKRF_TRUE) {
                nanosleep(&sleep_100m, NULL);
            }

            result = hackrf_stop_tx(tx_device);
            if( result != HACKRF_SUCCESS ) {
                fprintf(stderr, "hackrf_stop_tx() failed: %s (%d)\n",
                        hackrf_error_name((enum hackrf_error)result),
                        result);
            }
            // fprintf(stdout, "wake\n");
        }

        //while(hackrf_is_streaming(rx_device) == HACKRF_TRUE) {
          //  nanosleep(&sleep_100m, NULL);
        //}

        result = hackrf_stop_rx(rx_device);
        if( result != HACKRF_SUCCESS ) {
            fprintf(stderr, "hackrf_stop_rx() failed: %s (%d)\n",
                    hackrf_error_name((enum hackrf_error)result),
                    result);
        }
    }

    //// close tx device
    result = hackrf_close(tx_device);
    if(result != HACKRF_SUCCESS) {
        fprintf(stderr, "hackrf_close(tx) failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
    }

    //// close rx device
    result = hackrf_close(rx_device);
    if(result != HACKRF_SUCCESS) {
        fprintf(stderr, "hackrf_close(rx) failed: %s (%d)\n",
                hackrf_error_name((enum hackrf_error)result),
                result);
    }

    hackrf_exit();
}

