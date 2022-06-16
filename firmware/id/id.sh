#!/bin/bash

# Generate CAN ID bianry file from shell script

mkdir -p ./Objects
cd ./Objects
rm -f *.bin

for i in {0..2047}
do
    echo -n -e "\x$(printf '%02x' $((i & 0xFF)))\x$(printf '%02x' $(((i & 0xFF00) >> 8)))" > $i.bin
done