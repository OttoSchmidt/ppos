#!/bin/bash

BLOCK_QUANT=$1
CONTENT=""

for i in $(seq 1 $BLOCK_QUANT); do
	CONTENT="$CONTENT$(printf "<--bloco %04d-------------------------------------------------->" "$i")"
done

echo $CONTENT > hardware/disk.dat