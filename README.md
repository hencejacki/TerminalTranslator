# TerminalTranslator
Terminal translator(TTL). A translator for terminal use.

## Overview

The translate website is too slow and inconvenient to use, so build this tool.

## Platform

- [x] Linux
- [ ] Windows
- [ ] MacOS

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

3. Get source code of LibreTranslate(An offline translate api provider) and deploy it via [docs](https://github.com/LibreTranslate/LibreTranslate/blob/main/README.md)

4. Clone Repository

~~~bash
git clone https://github.com/hencejacki/TerminalTranslator.git
cd TerminalTranslator
~~~

5. Compile

~~~bash
mkdir build && cd build
# Default build type is debug.
# Add '-DCMAKE_BUILD_TYPE=Release' option to switch release version.
# To change install path, add '-DCMAKE_INSTALL_PREFIX=/path/to/xxx' option.
cmake ..
make && make install
# update path according to the prompt
# source ~/.bashrc
# or
# source ~/.config/fish/config.fish
~~~

6. Use

~~~bash
# Set your
ttl [-s|-t|-h|...] [word]
~~~

7. Uninstall

~~~bash
make uninstall
# Remember update your shell rc file
~~~

## Future work

- [ ] /translate_file
- [ ] /detect
- [ ] /languages
