#!/bin/bash
# ----------------------------------------------
# YONIX
# CU Sounding Rocket Lab - Avionics
#
# Title: BeagleBone Black cFS Script
# Written by: Jason Popich
#
# Starts the core flight system instance
#
# NOTE: Script must be run using sudo
# (i.e. sudo ./start-cfs.sh)
# ----------------------------------------------

# Mount the SD card if not already mounted
if grep -qs '/mnt/extsd ' /proc/mounts; then
	echo "/mnt/extsd already mounted... not mounting."
else
	mount -t vfat /dev/mmcblk0p1 /mnt/extsd
fi

# If /boot/SRL/arm-bbb already exists remove it.
if [ -d "/boot/SRL/arm-bbb" ]; then
	rm -rf /boot/SRL/arm-bbb
fi

# Copy cFS bundle from SD card to eMMC
cp -r /mnt/extsd/arm-bbb /boot/SRL/

# Change permissions
chmod -R 777 /boot/SRL/arm-bbb

# Move to cFS
cd /boot/SRL/arm-bbb

# Make ram directory if it doesn't exist
if [ ! -d "ram" ]; then
	mkdir ram
fi

# Execute cFS
./core-arm-bbb
