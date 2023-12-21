#!/bin/bash
file=$1
name="${file%.*}"
ext="${file##*.}"

size=$(stat -f%z "$name.bin")

Overlap=512
#Overlap=1024
Window=1024	# length of segment
#Window=2048	# length of segment
SizeOfFloat=4   # in bytes

K=$((($size/$Overlap/$SizeOfFloat - 1)/2))

psd $Overlap $K "$name" 
