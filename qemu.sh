#!/bin/sh
set -e

while getopts ":d:h" opt; do
  case $opt in
    d)
      . ./iso.sh -d
      qemu-system-$(./target-triplet-to-arch.sh $HOST) -serial file:"$OPTARG" -cdrom ${KERNEL_NAME}.iso
      ;;
    h) 
      echo "Usage: ./qemu.sh [OPTIONS]"
      echo "OPTIONS:"
      echo "  -d <logfile> Starts logging to a specified logfile."
      echo "               Note: You must specify a logfile. Try logfile.log"
      echo "  -h           Prints this help page."
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done


if [ $OPTIND -eq 1 ]; then
  . ./iso.sh
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom ${KERNEL_NAME}.iso
fi
