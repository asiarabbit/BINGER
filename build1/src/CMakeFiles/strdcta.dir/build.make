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
include src/CMakeFiles/strdcta.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/strdcta.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/strdcta.dir/flags.make

src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o: src/CMakeFiles/strdcta.dir/flags.make
src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o: ../src/strcali_dcta.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asia/pionExp2017/build1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o"
	cd /home/asia/pionExp2017/build1/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/strdcta.dir/strcali_dcta.cxx.o -c /home/asia/pionExp2017/src/strcali_dcta.cxx

src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/strdcta.dir/strcali_dcta.cxx.i"
	cd /home/asia/pionExp2017/build1/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asia/pionExp2017/src/strcali_dcta.cxx > CMakeFiles/strdcta.dir/strcali_dcta.cxx.i

src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/strdcta.dir/strcali_dcta.cxx.s"
	cd /home/asia/pionExp2017/build1/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asia/pionExp2017/src/strcali_dcta.cxx -o CMakeFiles/strdcta.dir/strcali_dcta.cxx.s

src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.requires:

.PHONY : src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.requires

src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.provides: src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.requires
	$(MAKE) -f src/CMakeFiles/strdcta.dir/build.make src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.provides.build
.PHONY : src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.provides

src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.provides.build: src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o


# Object files for target strdcta
strdcta_OBJECTS = \
"CMakeFiles/strdcta.dir/strcali_dcta.cxx.o"

# External object files for target strdcta
strdcta_EXTERNAL_OBJECTS =

strdcta: src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o
strdcta: src/CMakeFiles/strdcta.dir/build.make
strdcta: lib/libetf.so
strdcta: /home/asia/Documents/root/install/lib/libCore.so
strdcta: /home/asia/Documents/root/install/lib/libRIO.so
strdcta: /home/asia/Documents/root/install/lib/libNet.so
strdcta: /home/asia/Documents/root/install/lib/libHist.so
strdcta: /home/asia/Documents/root/install/lib/libGraf.so
strdcta: /home/asia/Documents/root/install/lib/libGraf3d.so
strdcta: /home/asia/Documents/root/install/lib/libGpad.so
strdcta: /home/asia/Documents/root/install/lib/libTree.so
strdcta: /home/asia/Documents/root/install/lib/libRint.so
strdcta: /home/asia/Documents/root/install/lib/libPostscript.so
strdcta: /home/asia/Documents/root/install/lib/libMatrix.so
strdcta: /home/asia/Documents/root/install/lib/libPhysics.so
strdcta: /home/asia/Documents/root/install/lib/libMathCore.so
strdcta: /home/asia/Documents/root/install/lib/libThread.so
strdcta: /home/asia/Documents/root/install/lib/libMultiProc.so
strdcta: src/CMakeFiles/strdcta.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asia/pionExp2017/build1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../strdcta"
	cd /home/asia/pionExp2017/build1/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/strdcta.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/strdcta.dir/build: strdcta

.PHONY : src/CMakeFiles/strdcta.dir/build

src/CMakeFiles/strdcta.dir/requires: src/CMakeFiles/strdcta.dir/strcali_dcta.cxx.o.requires

.PHONY : src/CMakeFiles/strdcta.dir/requires

src/CMakeFiles/strdcta.dir/clean:
	cd /home/asia/pionExp2017/build1/src && $(CMAKE_COMMAND) -P CMakeFiles/strdcta.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/strdcta.dir/clean

src/CMakeFiles/strdcta.dir/depend:
	cd /home/asia/pionExp2017/build1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asia/pionExp2017 /home/asia/pionExp2017/src /home/asia/pionExp2017/build1 /home/asia/pionExp2017/build1/src /home/asia/pionExp2017/build1/src/CMakeFiles/strdcta.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/strdcta.dir/depend
