![SRL Logo](https://www.colorado.edu/studentgroups/cobra/sites/default/files/styles/slider/public/slider/logo_crop.png?itok=jh4F9ZcZ)

# SRL_cFS
#### Table of contents
1. [Introduction](#Introduction)
    - [Team Members](#Team-Members)
2. [Compilation Instructions](#Compilation-Instructions)
    - [Instructions for cFS compilation on Raspberry PI - Development ONLY](#Instructions-for-cFS-compilation-on-Raspberry-PI---Development-ONLY)
    - [Instructions for Development Machine Setup](#Instructions-for-Development-Machine-Setup)
    - [Instructions for cFS compiliation](#Instructions-for-cFS-compiliation)
    - [Instructions for flashing cFS unto the BeagleBone Black](#Instructions-for-flashing-cFS-unto-the-BeagleBone-Black)
3. [Code Version Notes](#Code-Version-Notes)

## Introduction
This is the core Flight System repository for the Avionics team of the [Sounding Rocket Laboratory](https://www.colorado.edu/studentgroups/cobra/ "CU Sounding Rocket Laboratory Website") at the University of Colorado, Boulder.

The core Flight System is a Flight Software architecture framework that was designed to be used on a multitude of spacecraft. The CU Sounding Rocket Laboratory uses it as the basis for the Flight Software that is to be used on the Avionics suite.

**NOTE: Development and Compilation of cFS MUST happen on a linux system, however files can be edited and viewed on a seperate system whether that be Windows, Mac OS, or Linux**

**NOTE: Firewall on the development system must be disabled to allow comms from the ground station in / comms out.**

### Team Members
* [Lyon Foster](https://github.com/LyonFoster "LyonFoster")
* [Jason Popich](https://github.com/japopich "japopich")
* [Zoe Roy](https://github.com/zroy33 "zroy33")
* [Giselle Koo](https://github.com/gisellegk "gisellegk")
* [Brendan Haines](https://github.com/brendanhaines "brendanhaines")

---
## Compilation Instructions

### Instructions for cFS compilation on Raspberry PI - Development ONLY
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

---	
### Instructions for Development Machine Setup
There are several things that must be installed on a development machine for cFS. First and foremost your development machine must be running some sort of Linux distribution with OpenSUSE LEAP 15.2, CentOS 8, Ubuntu, RaspbianOS, and Arch Linux having been tested and confirmed working with cFS. 

#### GCC
Second off, gcc-7.5.0 must be installed on said development machine with the sole exception of OpenSUSE LEAP 15.2 as gcc-7.5.0 already comes with the OS. You can check what version your OS is running if you execute the following command:

	# Check gcc version
	$ gcc --version

If not running gcc-7.5.0, you can acquire gcc-7.5.0 from the [gcc mirrors](https://gcc.gnu.org/mirrors.html "gcc") /mirrors/gcc/releases/gcc-7.5.0 while also following the following instructions with the version number in the ${GCC_VERSION} location:

	# Unpack gcc directory that was acquired
	tar -xf gcc-${GCC_VERSION}.tar.bz2

	# download the prerequisites
	cd gcc-${GCC_VERSION}
	./contrib/download_prerequisites

	# create the build directory
	cd ..
	mkdir gcc-build
	cd gcc-build

	# build
	../gcc-${GCC_VERSION}/configure                      \
    		--prefix=/usr/bin                           	 \
    		--enable-shared                                  \
    		--enable-threads=posix                           \
    		--enable-__cxa_atexit                            \
    		--enable-clocale=gnu                             \
    		--enable-languages=all                           \
	&& make \
	&& make install
	
	# 

	# Notes
	#
	#   --enable-shared --enable-threads=posix --enable-__cxa_atexit: 
	#       These parameters are required to build the C++ libraries to published standards.
	#   
	#   --enable-clocale=gnu: 
	#       This parameter is a failsafe for incomplete locale data.
	#   
	#   --disable-multilib: 
	#       This parameter ensures that files are created for the specific
	#       architecture of your computer.
	#        This will disable building 32-bit support on 64-bit systems where the
	#        32 bit version of libc is not installed and you do not want to go
	#        through the trouble of building it. Diagnosis: "Compiler build fails
	#        with fatal error: gnu/stubs-32.h: No such file or directory"
	#   
	#   --with-system-zlib: 
	#       Uses the system zlib instead of the bundled one. zlib is used for
	#       compressing and uncompressing GCC's intermediate language in LTO (Link
	#       Time Optimization) object files.
	#   
	#   --enable-languages=all
	#   --enable-languages=c,c++,fortran,go,objc,obj-c++: 
	#       This command identifies which languages to build. You may modify this
	#       command to remove undesired language

#### BeagleBone Black ARM Compiler
Once the GCC is setup for the environment the ARM compiler must also be setup in order to cross-compile onto the BeagleBone Black. Installation is relatively easy.
You must first get the Linaro toolchain from [here](https://www.linaro.org/downloads/ linaro). **MAKE SURE YOU GET THE arm-linux-gnueabihf**

	# After you have downloaded the toolchain unzip it
	$ tar -xvf <TOOLCHAIN DOWNLOAD>
	
	# Then move the extracted output to the /opt/ folder.
	$ mv -r <EXTRACTED OUTPUT> /opt/gcc-arm-linux/
	
	# Add it to the path
	$ sudo vim /etc/profile.d/env.sh
	$ export PATH=$PATH:/opt/gcc-arm-linux/bin
	
	# Check to make sure it was added and if a version comes up it works.
	$ arm-linux-gnueabihf-gcc --version

---
### Instructions for cFS compiliation
These are instructions on how to build the core Flight System. Similiar steps are taken for different operating systems but it is up to the user to modify as needed. These steps must be taken everytime a change has been made and the repository must be rebuilt. **Furthermore flight ready code must be compiled on this machine with the BUILDTYPE=release and OMIT-DEPRECATED set in order to avoid any issues during flight.**

	# Change directory to SRL_CFS after you have cloned it from this repo
	$ cd SRL_CFS

	# Prep the repository for build (If on a system WITH gcc-7.5.0)
	$ make BUILDTYPE=release OMIT_DEPRECATED=true prep

	# Prep the repository for build (If on a system that does NOT have gcc-7.5.0)
	# The respective toolchain-cpu*.cmake files must have /opt/gcc-7.5.0/bin/gcc and /opt/gcc-7.5.0/bin/g++ in the CMAKE_C_COMPILER and CMAKE_CXX_COMPILER lines
	$ make BUILDTYPE=release OMIT_DEPRECATED=true CC=/opt/gcc-7.5.0/bin/gcc CXX=/opt/gcc-7.5.0/bin/g++ prep
	
	# Make the repository with 4 processes
	$ make -j 4

	# Install the exectuable
	$ sudo make install
	
	# Go to the executable
	$ cd build/exe/cpu1
	
	# Run the executable with executive permissions because needs special access
	$ sudo ./core-cpu1

---
### Instructions for flashing cFS unto the BeagleBone Black
Flashing onto the BeagleBone Black is super simple. In order to do so the BBB must first be setup using the setupBBB.sh script found in the INSTALL directory.

**NOTE: An SD card is required every boot. The bigger the SD card the better with a max of 32Gb (for now until tested higher).**

#### Setting up the BeagleBone Black
You need a computer capable of Serial Communication. Ideally Mac or Linux computers as they are the easiest to use because of in built terminal commands. For Windows computers google the equivalent of the following commands.

Once your computer is connected to the BBB pull up a terminal and determine which USB port corresponds to your BBB and then connect to it using a baud of 115200 by running the following commands.

	# List Devices, find the tty.usbxxx where xxx is something
	$ ls -al /dev/
	
	# Connect to it
	$ sudo screen -L /dev/tty.usbxxx 115200
	
	# You should now see the BeagleBoneBlack, the username is debian and password is temppwd

After a successful connection it is now time to prep the SD, and get ready for SRL installation using the provided scripts in the INSTALL directory. The first step in this process is by formatting the SD card as FAT32. **BE VERY CAREFUL WHEN FORMATTING AS YOU DON'T WANT TO ACCIDENTALLY REFORMAT YOUR DRIVE IF NEEDED ASK FOR HELP.** On Mac OS, this can be done by executing the following commands in the terminal:

	# Execute the following command to identify the mount name of SD card following the pattern /dev/diskX, e.g., /dev/disk2
	$ diskutil list 
	
	# Say the disk name is /dev/disk2. Now format the card to FAT32 by running the following command:
	$ sudo diskutil eraseDisk FAT32 MYSD MBRFormat /dev/disk2
	
On Linux use fdisk, the commands are the following and use the following [guide](https://ragnyll.gitlab.io/2018/05/22/format-a-sd-card-to-fat-32linux.html FAT32)

For Windows, use the in built windows disk formatter. Just right click the SD card, select FAT32, and let it go.

Once the SD card has been formatted, you need to copy the INSTALL folder into the SD card then place the mini-sd into the BeagleBone Black and reboot the BeagleBone Black. After that is done you need to mount the SD card tho only this first time as the INSTALL script will creates services that will mount the SD on boot. In order to mount the SD card do the following commands in the BBB:

	# Create mount point
	$ sudo mkdir /mnt/extsd
	
	# Mount the SD card
	$ sudo mount -t vfat /dev/mmcblk0p1 /mnt/extsd
	
	# If you get an error the issue is that /dev/mmcblk0p1 is probably not your SD card, 
	# go and find what it is and replace it accordingly in the command and then in the start-cfs.sh script inside the INSTALL/scripts folder.
	
	# Once that is done you can now run the install script
	$ cd /mnt/extsd/INSTALL
	$ sudo ./setupBBB.sh
	
	# After it is done your BeagleBone will reboot and you can double check if it worked because it'll automatically log you in. 
	# As well as 2 services will be started which can be checked with the following commands:
	
	# Check Services
	sudo systemctl status enable-pins.service
	sudo systemctl status start-cfs.service <--------- THIS ONE MAY NOT BE STARTED DEPENDING ON IF THE arm-bbb FOLDER EXISTS ON THE SD CARD
	
And your BeagleBone Black is now setup and ready to be used with cFS.

#### BeagleBone Black core Flight System Compilation and loading.

Compiling the core Flight System for use on the BeagleBone Black ( BB ) is very straight forward assuming you have correctly setup the BBB according to the earlier guide. The first thing you need to do is let cmake know to compile for the ARM processor not x86-x64 processor. In order to do that you have to change the target definitions in the yonix_defs folder.

	# Go to the yonix_defs folder
	cd yonix_defs
	
Open the targets.cmake file, comment out SET(TGT1_NAME cpu1) and uncomment #SET(TGT1_NAME arm-bbb) such that your file now looks like the following:
	
	#SET(TGT1_NAME cpu1)
	SET(TGT1_NAME arm-bbb)
	
Then save, exit, and compile cFS like you normally would. Afterwhich you know need to attach the SD card you were using to setup the BBB to your computer again. Then just drag and drop the arm-bbb folder from build/exe/ into your SD card root folder. Finally eject that SD card, place into BBB, and power on the BBB. cFS should now automatically start and you can check that by executing the following command on the BBB:

	sudo systemctl status start-cfs.service
	
If you want a realtime log of the cFS instance running, (i.e. you want to see the terminal output of cFS while it's running in real-time), execute the following:

	sudo journalctl -f -u start-cfs.service
	
If you want to stop the cFS instance, execute the following:

	sudo systemctl stop start-cfs.service
	
And if you need to do anything else, google it because cFS is running as a regular process under a service so there is nothing special required.

---
## Code Version Notes

6.7.0a - core Flight System release
