![SRL Logo](https://www.colorado.edu/studentgroups/cobra/sites/default/files/styles/slider/public/slider/logo_crop.png?itok=jh4F9ZcZ)

# SRL_cFS

This is the core Flight System repository for the Avionics team of the [Sounding Rocket Laboratory](https://www.colorado.edu/studentgroups/cobra/ "CU Sounding Rocket Laboratory Website") at the University of Colorado, Boulder. 
The core Flight System is a Flight Software architecture framework that was designed to be used on amultitude of spacecraft. The CU Sounding Rocket Laboratory uses it as the basis for the Flight Software that is to be used on the Avionics suite. 

## Team Members
* [Lyon Foster](https://github.com/LyonFoster "LyonFoster")
* [Jason Popich](https://github.com/japopich "japopich")
* [Zoe Roy](https://github.com/zroy33 "zroy33")
* [Carter Mak](https://github.com/cartermak "cartermak")
* [Giselle Koo](https://github.com/gisellegk "gisellegk")
* [Brendan Haines](https://github.com/brendanhaines "brendanhaines")

---

## Instructions
These are instructions on how to build the core Flight System on the SRL Virtual Machine for development. Instructions on how to build for deployment on Microcontroller to come.

	make BUILDTYPE=release OMIT_DEPRECATED=true CC=/opt/gcc-7.5.0/bin/gcc CXX=/opt/gcc-7.5.0/bin/g++ prep

	make -j 4

	sudo make install

	cd build/exe/cpu1

	sudo ./core-cpu1

---
## Code Version Notes
