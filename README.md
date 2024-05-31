# TerminalTranslator
Terminal translator(TTL). A translator for terminal use.

## Overview

The translate website is too slow and inconvenient to use, so build this tool.

## Usage

1. Get source code of cpr(A simple http client library)

~~~bash
wget https://github.com/libcpr/cpr/archive/refs/tags/1.9.7.tar.gz
tar -xvf 1.9.7.tar.gz
~~~

2. Build and install it

~~~bash
cd cpr-1.9.7/
mkdir build && cd build
cmake ..
cmake --build . --parallel
sudo cmake --install .
~~~

3. Clone Repository

~~~bash
git clone https://github.com/hencejacki/TerminalTranslator.git
cd TerminalTranslator
~~~

4. Compile

~~~bash
make && make install
# update path according to the prompt
# source ~/.bashrc
# or
# source ~/.config/fish/config.fish
~~~

5. Use

~~~bash
ttl [-s|-t|-h|...] [word]
~~~

## Future work

[ ] /translate_file

[ ] /detect

[ ] /languages
