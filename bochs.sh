#!/bin/sh
set -e

. ./iso.sh
bochs -f bochs.config -q
