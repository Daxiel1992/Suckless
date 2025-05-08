#!/bin/sh
ARTIST=$(playerctl metadata artist --player=spotify)
TITLE=$(playerctl metadata title --player=spotify)
if [ "$ARTIST" == "" ]; then
	exit
else
	echo " $TITLE - $ARTIST"
fi
