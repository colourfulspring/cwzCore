V := @

BUILD_DIR := build
KERNEL_IMG := $(BUILD_DIR)/scene.img

.PHONY: all build clean qemu dos2unix

all:
	$(V)cmake --build $(BUILD_DIR) --target all

build:
	$(V)cmake --build $(BUILD_DIR) --target kernel

clean:
	$(V)cmake --build $(BUILD_DIR) --target clean

qemu:
	qemu-system-i386 -nographic -drive file=$(KERNEL_IMG)

dos2unix:
	find . -type f -exec dos2unix {} \;