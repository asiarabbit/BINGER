# to build the project
# transcribed by Asia SUN, asia.rabbit@163.com, Jan. 15, 2019
# Last modified: Jan. 15, 2019, by Asia SUN

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ETF)

# -w show no warnings; -W: show all the warnings
add_compile_options("-Wno-overloaded-virtual")

#----------------------------------------------------------------------------
# find Geant4 package, activating all available UI and Vis drivers by default
# you can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()


#----------------------------------------------------------------------------
# setup Geant4 include directories and compile definition
# setup include directory for this project
#

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# setup ROOT incluce directories and compile definition
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

set(LIB_LIST ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# add the executable, and link it to the Geant4 libraries
#
add_executable(ETF ETF.cc ${sources} ${headers})
target_link_libraries(ETF ${LIB_LIST})

add_executable(ETFbatch ETFbatch.cc ${sources} ${headers})
target_link_libraries(ETFbatch ${LIB_LIST})

#add_executable(ETFbatch ETFbatch.cc ${sources} ${headers})
#target_link_libraries(ETFbatch ${LIB_LIST})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build ETF. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(ETF_SCRIPTS
	ETF.in
	ETF.out
	gui.mac
	init.mac
	init_vis.mac
	run1.mac
	run2.mac
	run.mac
	vis.mac
	icons.mac
	run.png
	)

foreach(_script ${ETF_SCRIPTS})
	configure_file(
		${PROJECT_SOURCE_DIR}/${_script}
		${PROJECT_BINARY_DIR}/${_script}
		COPYONLY
	)
endforeach()
