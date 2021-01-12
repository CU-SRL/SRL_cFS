# RTEMS - DEVELOPMENT DOCUMENTATION (EXTREMELY WIP.... MISSING PSPs and BSPs)
#### Table of contents
1. [Introduction](#Introduction)
2. [RTEMS Compiler and BeagleBone Black Flasher](#RTEMS-Compiler-and-BeagleBone-Black-Flasher)
3. [TO DO](#TO-DO)

## Introduction
This documentation will go over how to setup the RTEMS environment as well as to how setup the toolchain files for RTEMS.

**NOTE: THIS CURRENTLY DOESN'T FULLY COMPILE BECAUSE THERE ARE MISSING PSPs AND BSPs WHICH ARE REQUIRED**

## RTEMS Compiler and BeagleBone Black Flasher
First the RTEMS compiler and the BeagleBone Black (BBB) flasher must be compiled and installed. This can thankfully be partially accomplished using a repository that was created by another developer. Finally some of the most common errors are packages that are missing simple google searches should fix them. The instructions are as follow:

	# Clone the repository
	$ git clone https://gitlab.com/c-mauderer/rtems-bbb.git
	
	# Move into directory
	$ cd rtems-bbb
	
	# Build the RTEMS compiler and BBB Flasher
	$ make setup
	
	# Add to the path by first creating a(n) env file
	$ sudo touch /etc/profile.d/env.sh
	
	# Add it to the file and source it to use
	$ su
	$ echo "export PATH=$PATH:${PWD}/rtems-bbb/install/rtems/6/bin" >> /etc/profile.d/env.sh
	$ exit
	$ source /etc/profile.d/env.sh

After this has completed you must now install RTEMS 5 with ARM support because the repository above installs RTEMS 6 which has yet to be supported. In order to install RTEMS 5 the following commands must be used:

	# Install the ARM RTEMS compiler
	$ mkdir -p ${HOME}/rtems-5
	$ git clone -b 5 git://git.rtems.org/rtems-source-builder.git
	$ cd rtems-source-builder/rtems
	$ ../source-builder/sb-set-builder --prefix=$HOME/rtems-5 5/rtems-arm
	
	# Bootstrap RTEMS
	$ git clone -b 5 git://git.rtems.org/rtems.git
	$ export PATH=$HOME/rtems-5/bin:$PATH
	$ (cd rtems && ./bootstrap)
	
	# Install the BeagleBone Black BSP
	$ mkdir beagleboneblack
	$ cd beagleboneblack
	
	$ ../rtems/configure --target=arm-rtems5 --enable-rtemsbsp=beagleboneblack --prefix=${HOME}/rtems-5/ --enable-tests —enable-posix —disable-networking --enable-cxx --disable-deprecated
	
	$ gmake all
	$ make install
	
	# Now you are ready to compile an RTEMS version of cFS for the BeagleBoneBlack
	
## TO DO
	1. Create the BeagleBone Black PSP and BSP in cFS
	2. Compile
	3. Create create-sdcardimage5.sh based off of the create-sdcardimage.sh from the rtems-bbb installation
		- Needs to be placed in the bin for the RTEMS 6 installation
		- Needs to use RTEMS 5 not 6
	4. Document...
