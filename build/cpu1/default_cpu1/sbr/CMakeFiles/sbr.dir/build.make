# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/SRL/SRL_cFS/cfe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/SRL/SRL_cFS/build/cpu1/default_cpu1

# Include any dependencies generated for this target.
include sbr/CMakeFiles/sbr.dir/depend.make

# Include the progress variables for this target.
include sbr/CMakeFiles/sbr.dir/progress.make

# Include the compile flags for this target's objects.
include sbr/CMakeFiles/sbr.dir/flags.make

sbr/CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.o: sbr/CMakeFiles/sbr.dir/flags.make
sbr/CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.o: /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_map_direct.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/SRL/SRL_cFS/build/cpu1/default_cpu1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object sbr/CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.o"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && /opt/gcc-7.5.0/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.o -c /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_map_direct.c

sbr/CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.i"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && /opt/gcc-7.5.0/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_map_direct.c > CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.i

sbr/CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.s"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && /opt/gcc-7.5.0/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_map_direct.c -o CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.s

sbr/CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.o: sbr/CMakeFiles/sbr.dir/flags.make
sbr/CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.o: /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_route_unsorted.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/SRL/SRL_cFS/build/cpu1/default_cpu1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object sbr/CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.o"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && /opt/gcc-7.5.0/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.o -c /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_route_unsorted.c

sbr/CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.i"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && /opt/gcc-7.5.0/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_route_unsorted.c > CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.i

sbr/CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.s"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && /opt/gcc-7.5.0/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/SRL/SRL_cFS/cfe/modules/sbr/src/cfe_sbr_route_unsorted.c -o CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.s

# Object files for target sbr
sbr_OBJECTS = \
"CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.o" \
"CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.o"

# External object files for target sbr
sbr_EXTERNAL_OBJECTS =

sbr/libsbr.a: sbr/CMakeFiles/sbr.dir/src/cfe_sbr_map_direct.c.o
sbr/libsbr.a: sbr/CMakeFiles/sbr.dir/src/cfe_sbr_route_unsorted.c.o
sbr/libsbr.a: sbr/CMakeFiles/sbr.dir/build.make
sbr/libsbr.a: sbr/CMakeFiles/sbr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/SRL/SRL_cFS/build/cpu1/default_cpu1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libsbr.a"
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && $(CMAKE_COMMAND) -P CMakeFiles/sbr.dir/cmake_clean_target.cmake
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sbr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sbr/CMakeFiles/sbr.dir/build: sbr/libsbr.a

.PHONY : sbr/CMakeFiles/sbr.dir/build

sbr/CMakeFiles/sbr.dir/clean:
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr && $(CMAKE_COMMAND) -P CMakeFiles/sbr.dir/cmake_clean.cmake
.PHONY : sbr/CMakeFiles/sbr.dir/clean

sbr/CMakeFiles/sbr.dir/depend:
	cd /home/SRL/SRL_cFS/build/cpu1/default_cpu1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/SRL/SRL_cFS/cfe /home/SRL/SRL_cFS/cfe/modules/sbr /home/SRL/SRL_cFS/build/cpu1/default_cpu1 /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr /home/SRL/SRL_cFS/build/cpu1/default_cpu1/sbr/CMakeFiles/sbr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sbr/CMakeFiles/sbr.dir/depend

