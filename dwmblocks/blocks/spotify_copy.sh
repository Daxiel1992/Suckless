#!/bin/sh
case $1 in
 	1) exec "st" -e "spt" ;;
	2) printf ""  && exit ;;
	3) ARTIST=$(playerctl metadata artist --player=spotify)
	   TITLE=$(playerctl metadata title --player=spotify)
	   printf " %s - %s" "$TITLE" "$ARTIST" ;;
esac
	
