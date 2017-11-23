##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for compilation of object files and dynamically-linked libraries.
#
# Created Time: 2017/10/15
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2017/10/18, SUN Yazhou
##################################################################################################

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(inc)
aux_source_directory(src LIB_SRCS)

add_library(libetf SHARED ${LIB_SRCS})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set_target_properties(libetf PROPERTIES OUTPUT_NAME "etf")
