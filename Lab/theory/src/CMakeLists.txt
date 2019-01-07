#############################################################################################
# Theoretical Analysis Code for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2018/11/26
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/11/28, SUN Yazhou
#############################################################################################

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS}) # ROOT header path
include_directories(${PROJECT_SOURCE_DIR}/eikonal/inc) # eikonal header path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIB_LIST libeik ${ROOT_LIBRARIES})

add_executable(test test.C)
target_link_libraries(test ${LIB_LIST})
