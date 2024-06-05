#!/bin/sh
set -e
. ./iso.sh

while getopts ":d:h" opt; do
  case $opt in
    d)
      qemu-system-$(./target-triplet-to-arch.sh $HOST) -serial file:"$OPTARG" -cdrom myos.iso
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
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom myos.iso
fi
