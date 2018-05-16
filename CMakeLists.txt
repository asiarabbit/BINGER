##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2017/10/18
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/5/16, SUN Yazhou
##################################################################################################

cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME BINGER)
project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
Set(CMAKE_CXX_FALGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_subdirectory(etf) # library path
add_subdirectory(src) # user-defined source file path

