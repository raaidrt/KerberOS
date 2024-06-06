#!/bin/sh
set -e
. ./headers.sh

# Parse command-line options
while getopts "d" opt; do
  case $opt in
    d)
      DEBUG=1
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
  esac
done

for PROJECT in $PROJECTS; do
  if [ -n "$DEBUG" ]; then
    (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE DEBUG=1 install)
  else
    (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
  fi
done