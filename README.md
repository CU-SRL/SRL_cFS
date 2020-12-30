![SRL Logo](https://www.colorado.edu/studentgroups/cobra/sites/default/files/styles/slider/public/slider/logo_crop.png?itok=jh4F9ZcZ)

# SRL_cFS
#### Table of contents
1. [Introduction](#Introduction)
    - [Team Members](#Team-Members)
2. [Compilation Instructions](#Compilation-Instructions)
    - [Instructions for cFS compilation on Raspberry PI (Development ONLY)](#Instructions-for-cFS-compilation-on-Raspberry-PI-(Development-ONLY))
    - [Instructions for Development Machine Setup](#Instructions-for-Development-Machine-Setup)
    - [Instructions for cFS compiliation on OpenSUSE LEAP 15.2 (SRL VM)](#Instructions-for-cFS-compiliation-on-OpenSUSE-LEAP-15.2-(SRL-VM))
    - [Instructions for flashing cFS unto the BeagleBone Black](#Instructions-for-flashing-cFS-unto-the-BeagleBone-Black)
    - [Instructions for COSMOS compilation and installation](#Instructions-for-COSMOS-compilation-and-installation)
3. [Code Version Notes](#Code-Version-Notes)

## Introduction
This is the core Flight System repository for the Avionics team of the [Sounding Rocket Laboratory](https://www.colorado.edu/studentgroups/cobra/ "CU Sounding Rocket Laboratory Website") at the University of Colorado, Boulder.

The core Flight System is a Flight Software architecture framework that was designed to be used on a multitude of spacecraft. The CU Sounding Rocket Laboratory uses it as the basis for the Flight Software that is to be used on the Avionics suite.

**NOTE: Development and Compilation of cFS MUST happen on a linux system, however files can be edited and viewed on a seperate system whether that be Windows, Mac OS, or Linux**

**NOTE: Development and Compilation of COSMOS can happen on any of the supported systems outlined by the [Ball Aerospace COSMOS documentation](https://cosmosrb.com "COSMOS")**

### Team Members
* [Lyon Foster](https://github.com/LyonFoster "LyonFoster")
* [Jason Popich](https://github.com/japopich "japopich")
* [Zoe Roy](https://github.com/zroy33 "zroy33")
* [Giselle Koo](https://github.com/gisellegk "gisellegk")
* [Brendan Haines](https://github.com/brendanhaines "brendanhaines")

---
## Compilation Instructions

### Instructions for cFS compilation on Raspberry PI (Development ONLY)
These are the instructions to compile cFS on a Raspberry Pi for development and to be able to push changes to the repo for compilation on the SRL VM.

These instructions were pulled from the [Alan's Tech Notes](http://alanstechnotes.blogspot.com/2018/11/how-to-compile-core-flight-system-on.html "Alan's Tech Notes")

First, you need to edit **sample_defs/toolchain-cpu1.cmake**

In the file below, change the **CMAKE_SYSTEM_PROCESSOR** to **arm**

Also comment out the last line that has the **“-m32”** switch

	--------------------------------------------------------------------------------------------------
	# This example toolchain file describes the cross compiler to use for
	# the target architecture indicated in the configuration file.

	# In this sample application, the "cross" toolchain is configured to
	# simply use the system native compiler with the "m32" switch to output
	# 32-bit code on a 64-bit system.  This will not be necessary in
	# future revisions.

	# Basic cross system configuration
	SET(CMAKE_SYSTEM_NAME           Linux)
	SET(CMAKE_SYSTEM_VERSION        1)
	SET(CMAKE_SYSTEM_PROCESSOR      arm)

	# Specify the cross compiler executables
	# Typically these would be installed in a home directory or somewhere
	# in /opt.  However in this example the system compiler is used.
	SET(CMAKE_C_COMPILER            "/usr/bin/gcc")
	SET(CMAKE_CXX_COMPILER          "/usr/bin/g++")

	# Configure the find commands
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM   NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY   NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE   NEVER)

	# These variable settings are specific to cFE/OSAL and determines which
	# abstraction layers are built when using this toolchain
	SET(CFE_SYSTEM_PSPNAME      "pc-linux")
	SET(OSAL_SYSTEM_BSPNAME     "pc-linux")
	SET(OSAL_SYSTEM_OSTYPE      "posix")

	# This adds the "-m32" flag to all compile commands
	# SET(CMAKE_C_FLAGS_INIT "-m32" CACHE STRING "C Flags required by platform")

Next, edit the tools/elf2cfetbl/CMakelists.txt file

You need to comment out the **“-m32”** switch as below:

	------------------------------------------------------------------------------------
	# force build as 32-bit
	# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
	add_executable(elf2cfetbl elf2cfetbl.c)

Now you can build the cFS using the cMake build system:

	$ make prep
	$ make install

If everything built OK, then you can run the cFS:

	$ cd build/exe/cpu1
	$ sudo ./core-cpu1.bin
	
### Instructions for Development Machine Setup

** TO DO **

### Instructions for cFS compiliation on OpenSUSE LEAP 15.2 (SRL VM)
These are instructions on how to build the core Flight System on the SRL Virtual Machine for development running OpenSUSE LEAP 15.2 . These steps must be taken everytime a change has been made and the repository must be rebuilt. **Furthermore flight ready code must be compiled on this machine with the BUILDTYPE=release and OMIT-DEPRECATED set in order to avoid any issues during flight.**

	# Change directory to SRL_CFS after you have cloned it from this repo
	$ cd SRL_CFS

	# Prep the repository for build
	$ make BUILDTYPE=release OMIT_DEPRECATED=true CC=/opt/gcc-7.5.0/bin/gcc CXX=/opt/gcc-7.5.0/bin/g++ prep
	
	# Make the repository with 4 processes
	$ make -j 4

	# Install the exectuable
	$ sudo make install
	
	# Go to the executable
	$ cd build/exe/cpu1
	
	# Run the executable with executive permissions because needs special access
	$ sudo ./core-cpu1

### Instructions for flashing cFS unto the BeagleBone Black

 ** TO DO **
 
### Instructions for COSMOS compilation and installation

** TO DO **

---
## Code Version Notes

** TO DO **
