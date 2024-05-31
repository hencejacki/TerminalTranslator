CC = g++
CFLAGS_DEBUG = -D _DEBUG -g
CFLAGS_RELEASE = -O3
INCLUDE_DIR = -I src/include
LINK_OPTS = -lcpr
SRC = src/main.cc
OUT = bin/ttl
INSTALL_DIR = $(HOME)/.local

CHECK_BASH := $(shell echo $$SHELL | grep -q "bash" && echo yes || echo no)
CHECK_FISH := $(shell echo $$SHELL | grep -q "fish" && echo yes || echo no)

all: prepare release
	
prepare:
	@if [ ! -d bin ]; then \
		echo "Creating bin directory..."; \
		mkdir bin; \
	fi

debug: prepare $(SRC)
	$(CC) $(CFLAGS_DEBUG) $(SRC) -o $(OUT) $(INCLUDE_DIR) $(LINK_OPTS)

release: prepare $(SRC)
	$(CC) $(CFLAGS_RELEASE) $(SRC) -o $(OUT) $(INCLUDE_DIR) $(LINK_OPTS)

install:
	@if [ ! -d $(INSTALL_DIR)/bin ]; then \
		mkdir -p $(INSTALL_DIR)/bin; \
	fi
	@echo "-- Installing: $(INSTALL_DIR)/$(OUT)"
	@cp $(OUT) $(INSTALL_DIR)/$(OUT)
	@if [ "$(CHECK_BASH)" = "yes" ]; then \
		echo "export PATH=\$$PATH:$(INSTALL_DIR)/bin" >> $(HOME)/.bashrc; \
		echo "Please run 'source ~/.bashrc' to update your PATH."; \
	elif [ "$(CHECK_FISH)" = "yes" ]; then \
		echo "set -x PATH \$$PATH $(INSTALL_DIR)/bin" >> $(HOME)/.config/fish/config.fish; \
		echo "Please run 'source ~/.config/fish/config.fish' to update your PATH."; \
	else \
		echo "Unsupported shell"; \
	fi

clean:
	rm -rf bin/
	rm -rf $(INSTALL_DIR)/$(OUT)
