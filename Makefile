
BIN_NAME = lsdup

#Compiler flags
CFLAGS = -Werror -Wall -std=gnu99 -mcx16 -pthread -O3
LFLAGS = $(CFLAGS)

#List files for dependencies specification
BUILD_DIR = build
C_FILES = $(wildcard *.c)
O_FILES = $(addprefix build/,$(notdir $(C_FILES:.c=.o)))

.PHONY: all clean
.SECONDARY: build

all: pre-build build post-build


#Building
pre-build:
	mkdir -p $(BUILD_DIR)

build: $(O_FILES)
	gcc $(LFLAGS) -o $(BIN_NAME) $^

build/%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

post-build:
	size -A $(BIN_NAME)
	@echo "====== Build Succeded ======\n\r"


clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN_NAME)

