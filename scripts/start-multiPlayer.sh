#!/bin/sh

# kill existing multiplayer instances
killall multiPlayer
# start a new one
~/raspivj/multiPlayer/bin/./multiPlayer &
exit 0
