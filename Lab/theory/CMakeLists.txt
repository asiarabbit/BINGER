#############################################################################################
# Theoretical Analysis Code for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2018/11/26
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/11/26, SUN Yazhou
#############################################################################################

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(PROJECT_NAME EIKONAL)
project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../cmake/Modules")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_subdirectory(eikonal)
add_subdirectory(src)
