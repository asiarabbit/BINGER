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
include src/CMakeFiles/t0.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/t0.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/t0.dir/flags.make

src/CMakeFiles/t0.dir/t0cali.C.o: src/CMakeFiles/t0.dir/flags.make
src/CMakeFiles/t0.dir/t0cali.C.o: ../src/t0cali.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/t0.dir/t0cali.C.o"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/t0.dir/t0cali.C.o -c /home/asia/pionExp2017/src/t0cali.C

src/CMakeFiles/t0.dir/t0cali.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/t0.dir/t0cali.C.i"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asia/pionExp2017/src/t0cali.C > CMakeFiles/t0.dir/t0cali.C.i

src/CMakeFiles/t0.dir/t0cali.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/t0.dir/t0cali.C.s"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asia/pionExp2017/src/t0cali.C -o CMakeFiles/t0.dir/t0cali.C.s

src/CMakeFiles/t0.dir/t0cali.C.o.requires:

.PHONY : src/CMakeFiles/t0.dir/t0cali.C.o.requires

src/CMakeFiles/t0.dir/t0cali.C.o.provides: src/CMakeFiles/t0.dir/t0cali.C.o.requires
	$(MAKE) -f src/CMakeFiles/t0.dir/build.make src/CMakeFiles/t0.dir/t0cali.C.o.provides.build
.PHONY : src/CMakeFiles/t0.dir/t0cali.C.o.provides

src/CMakeFiles/t0.dir/t0cali.C.o.provides.build: src/CMakeFiles/t0.dir/t0cali.C.o


# Object files for target t0
t0_OBJECTS = \
"CMakeFiles/t0.dir/t0cali.C.o"

# External object files for target t0
t0_EXTERNAL_OBJECTS =

t0: src/CMakeFiles/t0.dir/t0cali.C.o
t0: src/CMakeFiles/t0.dir/build.make
t0: lib/libetf.so
t0: /home/asia/Documents/root/install/lib/libCore.so
t0: /home/asia/Documents/root/install/lib/libRIO.so
t0: /home/asia/Documents/root/install/lib/libNet.so
t0: /home/asia/Documents/root/install/lib/libHist.so
t0: /home/asia/Documents/root/install/lib/libGraf.so
t0: /home/asia/Documents/root/install/lib/libGraf3d.so
t0: /home/asia/Documents/root/install/lib/libGpad.so
t0: /home/asia/Documents/root/install/lib/libTree.so
t0: /home/asia/Documents/root/install/lib/libRint.so
t0: /home/asia/Documents/root/install/lib/libPostscript.so
t0: /home/asia/Documents/root/install/lib/libMatrix.so
t0: /home/asia/Documents/root/install/lib/libPhysics.so
t0: /home/asia/Documents/root/install/lib/libMathCore.so
t0: /home/asia/Documents/root/install/lib/libThread.so
t0: /home/asia/Documents/root/install/lib/libMultiProc.so
t0: src/CMakeFiles/t0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../t0"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/t0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/t0.dir/build: t0

.PHONY : src/CMakeFiles/t0.dir/build

src/CMakeFiles/t0.dir/requires: src/CMakeFiles/t0.dir/t0cali.C.o.requires

.PHONY : src/CMakeFiles/t0.dir/requires

src/CMakeFiles/t0.dir/clean:
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -P CMakeFiles/t0.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/t0.dir/clean

src/CMakeFiles/t0.dir/depend:
	cd /home/asia/pionExp2017/anaG4SimBuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asia/pionExp2017 /home/asia/pionExp2017/src /home/asia/pionExp2017/anaG4SimBuild /home/asia/pionExp2017/anaG4SimBuild/src /home/asia/pionExp2017/anaG4SimBuild/src/CMakeFiles/t0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/t0.dir/depend
