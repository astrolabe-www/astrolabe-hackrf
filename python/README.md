#### dependencies
- install [libhackrf](https://github.com/mossmann/hackrf/tree/master/host)
- `git clone https://github.com/dressel/pyhackrf.git`
- `pip install matplotlib`


#### tx notes
- [py-hackrf-ctypes](https://github.com/wzyy2/py-hackrf-ctypes) for tx usage
- [hackrf tx_callback](https://github.com/mossmann/hackrf/blob/master/host/hackrf-tools/src/hackrf_transfer.c)
- returns -1 when done, 0 when should keep going
- called with: `hackrf_start_tx(device, tx_callback, NULL);`
