##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2017/10/18
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/1/27, SUN Yazhou
##################################################################################################

cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME BINGER)
project(${PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
Set(CMAKE_CXX_FALGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_subdirectory(etf) # library path
add_subdirectory(src) # user-defined source file path
