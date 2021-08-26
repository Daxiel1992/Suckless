#!/bin/sh
NUM=$(yay -Qu | wc -l)
if [ $NUM = "" ]; then
	NUM="0"
fi
printf " %s" "$NUM"
