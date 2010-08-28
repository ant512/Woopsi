#!/bin/sh

export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM

Dirlist=$(ls -d */)
for direc in $Dirlist ; do
	cd $direc
	rm -rf Release
	make clean
	cd ..
done