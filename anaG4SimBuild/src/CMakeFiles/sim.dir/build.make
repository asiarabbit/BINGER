# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/asia/pionExp2017

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/asia/pionExp2017/anaG4SimBuild

# Include any dependencies generated for this target.
include src/CMakeFiles/sim.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/sim.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/sim.dir/flags.make

src/CMakeFiles/sim.dir/sim.C.o: src/CMakeFiles/sim.dir/flags.make
src/CMakeFiles/sim.dir/sim.C.o: ../src/sim.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/sim.dir/sim.C.o"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sim.dir/sim.C.o -c /home/asia/pionExp2017/src/sim.C

src/CMakeFiles/sim.dir/sim.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sim.dir/sim.C.i"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asia/pionExp2017/src/sim.C > CMakeFiles/sim.dir/sim.C.i

src/CMakeFiles/sim.dir/sim.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sim.dir/sim.C.s"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asia/pionExp2017/src/sim.C -o CMakeFiles/sim.dir/sim.C.s

src/CMakeFiles/sim.dir/sim.C.o.requires:

.PHONY : src/CMakeFiles/sim.dir/sim.C.o.requires

src/CMakeFiles/sim.dir/sim.C.o.provides: src/CMakeFiles/sim.dir/sim.C.o.requires
	$(MAKE) -f src/CMakeFiles/sim.dir/build.make src/CMakeFiles/sim.dir/sim.C.o.provides.build
.PHONY : src/CMakeFiles/sim.dir/sim.C.o.provides

src/CMakeFiles/sim.dir/sim.C.o.provides.build: src/CMakeFiles/sim.dir/sim.C.o


# Object files for target sim
sim_OBJECTS = \
"CMakeFiles/sim.dir/sim.C.o"

# External object files for target sim
sim_EXTERNAL_OBJECTS =

sim: src/CMakeFiles/sim.dir/sim.C.o
sim: src/CMakeFiles/sim.dir/build.make
sim: lib/libetf.so
sim: /home/asia/Documents/root/install/lib/libCore.so
sim: /home/asia/Documents/root/install/lib/libRIO.so
sim: /home/asia/Documents/root/install/lib/libNet.so
sim: /home/asia/Documents/root/install/lib/libHist.so
sim: /home/asia/Documents/root/install/lib/libGraf.so
sim: /home/asia/Documents/root/install/lib/libGraf3d.so
sim: /home/asia/Documents/root/install/lib/libGpad.so
sim: /home/asia/Documents/root/install/lib/libTree.so
sim: /home/asia/Documents/root/install/lib/libRint.so
sim: /home/asia/Documents/root/install/lib/libPostscript.so
sim: /home/asia/Documents/root/install/lib/libMatrix.so
sim: /home/asia/Documents/root/install/lib/libPhysics.so
sim: /home/asia/Documents/root/install/lib/libMathCore.so
sim: /home/asia/Documents/root/install/lib/libThread.so
sim: /home/asia/Documents/root/install/lib/libMultiProc.so
sim: src/CMakeFiles/sim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../sim"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/sim.dir/build: sim

.PHONY : src/CMakeFiles/sim.dir/build

src/CMakeFiles/sim.dir/requires: src/CMakeFiles/sim.dir/sim.C.o.requires

.PHONY : src/CMakeFiles/sim.dir/requires

src/CMakeFiles/sim.dir/clean:
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -P CMakeFiles/sim.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/sim.dir/clean

src/CMakeFiles/sim.dir/depend:
	cd /home/asia/pionExp2017/anaG4SimBuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asia/pionExp2017 /home/asia/pionExp2017/src /home/asia/pionExp2017/anaG4SimBuild /home/asia/pionExp2017/anaG4SimBuild/src /home/asia/pionExp2017/anaG4SimBuild/src/CMakeFiles/sim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/sim.dir/depend

