#!/bin/bash

build_dir=build
kernel_img=scene.img

if [ "$1" = "qemu" ]; then

    qemu-system-i386 -nographic \
    -drive file=$build_dir/$kernel_img

else
    echo "?"
fi

# Ctrl+A x