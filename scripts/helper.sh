#!/bin/bash

build_dir=build
boot_bin=boot.bin
rootfs_img=rootfs.img

if [ "$1" = "build" ]; then

    if [[ -e $build_dir ]]; then
        rm -rf $build_dir
    fi
    
    mkdir -p $build_dir
    nasm -o $build_dir/$boot_bin kernel/boot/boot.asm
    dd if=$build_dir/$boot_bin of=$build_dir/$rootfs_img bs=512 count=1

elif [ "$1" = "qemu" ]; then

    qemu-system-x86_64 -nographic \
    -drive file=$build_dir/$rootfs_img

else
    echo "?"
fi

# Ctrl+A x