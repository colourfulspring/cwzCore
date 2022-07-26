BUILD_DIR := ./build/
OUT_DIR := ./out/
CFLAGS = -O3
LDFLAGS = -lc
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib
endif
SRCS := $(shell find . -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

all: clean dirs fatfs_maker

dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OUT_DIR)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	clang -I. $(CFLAGS) -c $< -o $@

fatfs_maker: dirs $(OBJS)
	clang $(OBJS) $(LDFLAGS) -o $(OUT_DIR)/fatfs_maker

clean:
	rm -rf $(OUT_DIR) $(BUILD_DIR) >/dev/null 2>/dev/null || true
