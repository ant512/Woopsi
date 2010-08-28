#!/bin/sh

export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM

Dirlist=$(ls -d */)
for direc in $Dirlist ; do
	cd $direc
	make clean
	make
	cd ..
done