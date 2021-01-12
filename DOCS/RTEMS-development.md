# RTEMS - DEVELOPMENT DOCUMENTATION
#### Table of contents
1. [Introduction](#Introduction)
2. [RTEMS Compiler and BeagleBone Black Flasher](#RTEMS-Compiler-and-BeagleBone-Black-Flasher)

## Introduction
This documentation will go over how to setup the RTEMS environment as well as to how setup the toolchain files for RTEMS.

## RTEMS Compiler and BeagleBone Black Flasher
Finally the last thing that must be installed is RTEMS compiler and the BeagleBone Black (BBB) flasher. This can thankfully be accomplished using a repository that was created by another guy. **If you so choose to do this by yourself you must install the RTEMS compiler and the BeagleBone Black Flasher seperately using their provided instructions.** Finally some of the most common errors are packages that are missing simple google searches should fix them. The instructions are as follow:

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

