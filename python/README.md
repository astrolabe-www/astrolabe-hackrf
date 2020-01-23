#### dependencies
- `env PYTHON_CONFIGURE_OPTS="--enable-framework --enable-ipv6 --enable-unicode --with-threads" pyenv install 2.7.16`
- install [libhackrf](https://github.com/mossmann/hackrf/tree/master/host) from 208fae commit
  - git clone https://github.com/mossmann/hackrf && cd hackrf
  - git checkout -b 208fae 208fae
  - cd host && mkdir build && cd build && cmake ..
  - make && sudo make install
- `git submodule init && git submodule update`
- `pip install -r requirements.txt`
