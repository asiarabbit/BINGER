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
include src/CMakeFiles/strpdc.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/strpdc.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/strpdc.dir/flags.make

src/CMakeFiles/strpdc.dir/strcali_pdc.C.o: src/CMakeFiles/strpdc.dir/flags.make
src/CMakeFiles/strpdc.dir/strcali_pdc.C.o: ../src/strcali_pdc.C
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/strpdc.dir/strcali_pdc.C.o"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/strpdc.dir/strcali_pdc.C.o -c /home/asia/pionExp2017/src/strcali_pdc.C

src/CMakeFiles/strpdc.dir/strcali_pdc.C.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/strpdc.dir/strcali_pdc.C.i"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/asia/pionExp2017/src/strcali_pdc.C > CMakeFiles/strpdc.dir/strcali_pdc.C.i

src/CMakeFiles/strpdc.dir/strcali_pdc.C.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/strpdc.dir/strcali_pdc.C.s"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/asia/pionExp2017/src/strcali_pdc.C -o CMakeFiles/strpdc.dir/strcali_pdc.C.s

src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.requires:

.PHONY : src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.requires

src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.provides: src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.requires
	$(MAKE) -f src/CMakeFiles/strpdc.dir/build.make src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.provides.build
.PHONY : src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.provides

src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.provides.build: src/CMakeFiles/strpdc.dir/strcali_pdc.C.o


# Object files for target strpdc
strpdc_OBJECTS = \
"CMakeFiles/strpdc.dir/strcali_pdc.C.o"

# External object files for target strpdc
strpdc_EXTERNAL_OBJECTS =

strpdc: src/CMakeFiles/strpdc.dir/strcali_pdc.C.o
strpdc: src/CMakeFiles/strpdc.dir/build.make
strpdc: lib/libetf.so
strpdc: /home/asia/Documents/root/install/lib/libCore.so
strpdc: /home/asia/Documents/root/install/lib/libRIO.so
strpdc: /home/asia/Documents/root/install/lib/libNet.so
strpdc: /home/asia/Documents/root/install/lib/libHist.so
strpdc: /home/asia/Documents/root/install/lib/libGraf.so
strpdc: /home/asia/Documents/root/install/lib/libGraf3d.so
strpdc: /home/asia/Documents/root/install/lib/libGpad.so
strpdc: /home/asia/Documents/root/install/lib/libTree.so
strpdc: /home/asia/Documents/root/install/lib/libRint.so
strpdc: /home/asia/Documents/root/install/lib/libPostscript.so
strpdc: /home/asia/Documents/root/install/lib/libMatrix.so
strpdc: /home/asia/Documents/root/install/lib/libPhysics.so
strpdc: /home/asia/Documents/root/install/lib/libMathCore.so
strpdc: /home/asia/Documents/root/install/lib/libThread.so
strpdc: /home/asia/Documents/root/install/lib/libMultiProc.so
strpdc: src/CMakeFiles/strpdc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/asia/pionExp2017/anaG4SimBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../strpdc"
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/strpdc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/strpdc.dir/build: strpdc

.PHONY : src/CMakeFiles/strpdc.dir/build

src/CMakeFiles/strpdc.dir/requires: src/CMakeFiles/strpdc.dir/strcali_pdc.C.o.requires

.PHONY : src/CMakeFiles/strpdc.dir/requires

src/CMakeFiles/strpdc.dir/clean:
	cd /home/asia/pionExp2017/anaG4SimBuild/src && $(CMAKE_COMMAND) -P CMakeFiles/strpdc.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/strpdc.dir/clean

src/CMakeFiles/strpdc.dir/depend:
	cd /home/asia/pionExp2017/anaG4SimBuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asia/pionExp2017 /home/asia/pionExp2017/src /home/asia/pionExp2017/anaG4SimBuild /home/asia/pionExp2017/anaG4SimBuild/src /home/asia/pionExp2017/anaG4SimBuild/src/CMakeFiles/strpdc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/strpdc.dir/depend

