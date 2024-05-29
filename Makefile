CC = g++
CFLAGS_DEBUG = -D _DEBUG
INCLUDE_DIR = -I src/include
SRC = src/main.cc
OUT = bin/ttl

all: prepare debug
	
prepare:
	@if [ ! -d bin ]; then \
		echo "Creating bin directory..."; \
		mkdir bin; \
	fi

debug: prepare $(SRC)
	$(CC) $(CFLAGS_DEBUG) $(SRC) -o $(OUT) $(INCLUDE_DIR)

clean:
	rm -rf bin ttl
