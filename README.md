# TerminalTranslator
Terminal translator(TTL). A translator for terminal use.

## Overview

The translate website is too slow and inconvenient to use when i'm coding, so build this tool.

## Platform

- [x] Linux
- [ ] Windows
- [ ] MacOS

## Languages support

+ en -> zh
+ zh -> en

## Usage

1. Clone Repository

~~~bash
git clone https://github.com/hencejacki/TerminalTranslator.git
cd TerminalTranslator
~~~

2. Compile

~~~bash
mkdir build && cd build
# Default build type is debug.
# Add '-DCMAKE_BUILD_TYPE=Release' flag to switch release version.
# Default install path is /usr/local
# To change install path, add '-DCMAKE_INSTALL_PREFIX=/path/to/xxx' flag.
cmake ..
make && make install
# update path according to the prompt
# source ~/.bashrc
# or
# source ~/.config/fish/config.fish
~~~

3. Use

~~~bash
# Set your custom command use alias
ttl [-s|-t|-h|...] [word]
~~~

4. Uninstall

~~~bash
make uninstall
# Remember update your shell rc file
~~~

## Future work

- [ ] /translate_file
- [ ] /detect
- [ ] /languages
