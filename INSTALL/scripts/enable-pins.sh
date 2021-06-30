#!/bin/bash
# ----------------------------------------------
# YONIX
# CU Sounding Rocket Lab - Avionics
#
# Title: BeagleBone Black Pin Config Script
# Written by: Jason Popich
#
#
# Enables I2C, UART, and SPI pins
#
# ----------------------------------------------

# Enable I2C
config-pin p9.19 i2c
config-pin p9.20 i2c

# Enable UART

# Enable SPI
config-pin p9_17 spi_cs
config-pin p9_18 spi
config-pin p9_21 spi
config-pin p9_22 spi_sclk