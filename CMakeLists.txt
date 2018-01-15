##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2017/10/18
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/1/10, SUN Yazhou
##################################################################################################

cmake_minimum_required(VERSION 3.6)
set(PROJECT_NAME BINGER)
project(${PROJECT_NAME})

add_compile_options(-g) # for gdb debugging

add_subdirectory(etf) # library path
add_subdirectory(src) # user-defined source file path
