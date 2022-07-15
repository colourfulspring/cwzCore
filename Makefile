V := @

.PHONY: all build qemu

all:
	$(V)echo "?"

build:
	$(V)scripts/helper.sh build

qemu:
	$(V)scripts/helper.sh qemu

dos2unix:
	find . -type f -exec dos2unix {} \;