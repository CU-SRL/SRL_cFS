#!/bin/bash
# ----------------------------------------------
# YONIX
# CU Sounding Rocket Lab - Avionics
# 
# Title: BeagleBone Black Setup Script
# Written by: Jason Popich
#
#
# Sets up a BBB with all the required scripts
# and system settings to run a flight capable
# core Flight System instance.
#
# NOTE: Script must be run using sudo 
# (i.e. sudo ./setupBBB.sh)
# ----------------------------------------------

# Create mount directory for SD card
if [ ! -d "/mnt/extsd" ]; then
	mkdir /mnt/extsd
fi

# Create SRL file structure in /boot
if [ ! -d "/boot/SRL" ]; then
	mkdir /boot/SRL
fi

# Copy Auto-Startup Scripts
cp scripts/enable-pins.sh /boot/SRL/
cp scripts/start-cfs.sh /boot/SRL/

# Copy systemd service scripts
cp systemd/enable-pins.service /lib/systemd/system/
cp systemd/start-cfs.service /lib/systemd/system/

# Change permissions to scripts
chmod 777 /boot/SRL/enable-pins.sh
chmod 777 /boot/SRL/start-cfs.sh

# Enable the Services
systemctl daemon-reload
systemctl enable enable-pins.service
systemctl enable start-cfs.service

# Disable sudo passwd
#sed -i '/ALL=(ALL:ALL)/a debian ALL=NOPASSWD: ALL' /etc/sudoers

# BBB debian auto login
#sed -i 's/\bExecStart=-\/sbin\/agetty\b/& -a debian/' /lib/systemd/system/serial-getty@.service

# Update the MOTD


# Reboot
reboot
