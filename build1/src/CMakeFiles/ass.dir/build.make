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
CMAKE_BINARY_DIR = /home/asia/pionExp2017/build1

# Include any dependencies generated for this target.
include src/CMakeFiles/ass.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ass.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ass.dir/flags.make

src/CMakeFiles/ass.dir/assess.C.o: src/CMakeFiles/ass.dir/flags.make
src/CMakeFiles/ass.dir/assess.C.o: ../src/assess.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asia/pionExp2017/build1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/ass.dir/assess.C.o"
	cd /home/asia/pionExp2017/build1/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ass.dir/assess.C.o -c /home/asia/pionExp2017/src/assess.C

src/CMakeFiles/ass.dir/assess.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ass.dir/assess.C.i"
	cd /home/asia/pionExp2017/build1/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asia/pionExp2017/src/assess.C > CMakeFiles/ass.dir/assess.C.i

src/CMakeFiles/ass.dir/assess.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ass.dir/assess.C.s"
	cd /home/asia/pionExp2017/build1/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asia/pionExp2017/src/assess.C -o CMakeFiles/ass.dir/assess.C.s

src/CMakeFiles/ass.dir/assess.C.o.requires:

.PHONY : src/CMakeFiles/ass.dir/assess.C.o.requires

src/CMakeFiles/ass.dir/assess.C.o.provides: src/CMakeFiles/ass.dir/assess.C.o.requires
	$(MAKE) -f src/CMakeFiles/ass.dir/build.make src/CMakeFiles/ass.dir/assess.C.o.provides.build
.PHONY : src/CMakeFiles/ass.dir/assess.C.o.provides

src/CMakeFiles/ass.dir/assess.C.o.provides.build: src/CMakeFiles/ass.dir/assess.C.o


# Object files for target ass
ass_OBJECTS = \
"CMakeFiles/ass.dir/assess.C.o"

# External object files for target ass
ass_EXTERNAL_OBJECTS =

ass: src/CMakeFiles/ass.dir/assess.C.o
ass: src/CMakeFiles/ass.dir/build.make
ass: lib/libetf.so
ass: /home/asia/Documents/root/install/lib/libCore.so
ass: /home/asia/Documents/root/install/lib/libRIO.so
ass: /home/asia/Documents/root/install/lib/libNet.so
ass: /home/asia/Documents/root/install/lib/libHist.so
ass: /home/asia/Documents/root/install/lib/libGraf.so
ass: /home/asia/Documents/root/install/lib/libGraf3d.so
ass: /home/asia/Documents/root/install/lib/libGpad.so
ass: /home/asia/Documents/root/install/lib/libTree.so
ass: /home/asia/Documents/root/install/lib/libRint.so
ass: /home/asia/Documents/root/install/lib/libPostscript.so
ass: /home/asia/Documents/root/install/lib/libMatrix.so
ass: /home/asia/Documents/root/install/lib/libPhysics.so
ass: /home/asia/Documents/root/install/lib/libMathCore.so
ass: /home/asia/Documents/root/install/lib/libThread.so
ass: /home/asia/Documents/root/install/lib/libMultiProc.so
ass: src/CMakeFiles/ass.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asia/pionExp2017/build1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../ass"
	cd /home/asia/pionExp2017/build1/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ass.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ass.dir/build: ass

.PHONY : src/CMakeFiles/ass.dir/build

src/CMakeFiles/ass.dir/requires: src/CMakeFiles/ass.dir/assess.C.o.requires

.PHONY : src/CMakeFiles/ass.dir/requires

src/CMakeFiles/ass.dir/clean:
	cd /home/asia/pionExp2017/build1/src && $(CMAKE_COMMAND) -P CMakeFiles/ass.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ass.dir/clean

src/CMakeFiles/ass.dir/depend:
	cd /home/asia/pionExp2017/build1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asia/pionExp2017 /home/asia/pionExp2017/src /home/asia/pionExp2017/build1 /home/asia/pionExp2017/build1/src /home/asia/pionExp2017/build1/src/CMakeFiles/ass.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ass.dir/depend

