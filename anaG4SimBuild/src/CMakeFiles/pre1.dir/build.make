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
include src/CMakeFiles/pre1.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/pre1.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/pre1.dir/flags.make

src/CMakeFiles/pre1.dir/pre1.C.o: src/CMakeFiles/pre1.dir/flags.make
src/CMakeFiles/pre1.dir/pre1.C.o: ../src/pre1.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/pre1.dir/pre1.C.o"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pre1.dir/pre1.C.o -c /home/asia/pionExp2017/src/pre1.C

src/CMakeFiles/pre1.dir/pre1.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pre1.dir/pre1.C.i"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asia/pionExp2017/src/pre1.C > CMakeFiles/pre1.dir/pre1.C.i

src/CMakeFiles/pre1.dir/pre1.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pre1.dir/pre1.C.s"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asia/pionExp2017/src/pre1.C -o CMakeFiles/pre1.dir/pre1.C.s

src/CMakeFiles/pre1.dir/pre1.C.o.requires:

.PHONY : src/CMakeFiles/pre1.dir/pre1.C.o.requires

src/CMakeFiles/pre1.dir/pre1.C.o.provides: src/CMakeFiles/pre1.dir/pre1.C.o.requires
	$(MAKE) -f src/CMakeFiles/pre1.dir/build.make src/CMakeFiles/pre1.dir/pre1.C.o.provides.build
.PHONY : src/CMakeFiles/pre1.dir/pre1.C.o.provides

src/CMakeFiles/pre1.dir/pre1.C.o.provides.build: src/CMakeFiles/pre1.dir/pre1.C.o


# Object files for target pre1
pre1_OBJECTS = \
"CMakeFiles/pre1.dir/pre1.C.o"

# External object files for target pre1
pre1_EXTERNAL_OBJECTS =

pre1: src/CMakeFiles/pre1.dir/pre1.C.o
pre1: src/CMakeFiles/pre1.dir/build.make
pre1: lib/libetf.so
pre1: /home/asia/Documents/root/install/lib/libCore.so
pre1: /home/asia/Documents/root/install/lib/libRIO.so
pre1: /home/asia/Documents/root/install/lib/libNet.so
pre1: /home/asia/Documents/root/install/lib/libHist.so
pre1: /home/asia/Documents/root/install/lib/libGraf.so
pre1: /home/asia/Documents/root/install/lib/libGraf3d.so
pre1: /home/asia/Documents/root/install/lib/libGpad.so
pre1: /home/asia/Documents/root/install/lib/libTree.so
pre1: /home/asia/Documents/root/install/lib/libRint.so
pre1: /home/asia/Documents/root/install/lib/libPostscript.so
pre1: /home/asia/Documents/root/install/lib/libMatrix.so
pre1: /home/asia/Documents/root/install/lib/libPhysics.so
pre1: /home/asia/Documents/root/install/lib/libMathCore.so
pre1: /home/asia/Documents/root/install/lib/libThread.so
pre1: /home/asia/Documents/root/install/lib/libMultiProc.so
pre1: src/CMakeFiles/pre1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../pre1"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pre1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/pre1.dir/build: pre1

.PHONY : src/CMakeFiles/pre1.dir/build

src/CMakeFiles/pre1.dir/requires: src/CMakeFiles/pre1.dir/pre1.C.o.requires

.PHONY : src/CMakeFiles/pre1.dir/requires

src/CMakeFiles/pre1.dir/clean:
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -P CMakeFiles/pre1.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/pre1.dir/clean

src/CMakeFiles/pre1.dir/depend:
	cd /home/asia/pionExp2017/anaG4SimBuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asia/pionExp2017 /home/asia/pionExp2017/src /home/asia/pionExp2017/anaG4SimBuild /home/asia/pionExp2017/anaG4SimBuild/src /home/asia/pionExp2017/anaG4SimBuild/src/CMakeFiles/pre1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/pre1.dir/depend
