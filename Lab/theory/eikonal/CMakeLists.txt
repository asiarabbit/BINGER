#############################################################################################
# Theoretical Analysis Code for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for compilation of object files and dynamically-linked libraries.
#
# Created Time: 2018/11/26
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/11/26, SUN Yazhou
#############################################################################################

#find_package(ROOT REQUIRED)
#include_directories(${ROOT_INCLUDE_DIRS})
include_directories(inc)
aux_source_directory(src LIB_SRCS)

add_library(libeik SHARED ${LIB_SRCS})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(libeik PROPERTIES OUTPUT_NAME "eik")
