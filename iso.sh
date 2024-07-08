#!/bin/sh
set -e
. ./build.sh "$@"

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/${KERNEL_NAME}.kernel isodir/boot/${KERNEL_NAME}.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "${KERNEL_NAME}" {
	multiboot /boot/${KERNEL_NAME}.kernel
}
EOF
grub-mkrescue -o ${KERNEL_NAME}.iso isodir
