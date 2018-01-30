##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2017/10/18
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/1/10, SUN Yazhou
##################################################################################################

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS}) # ROOT header path
include_directories(${PROJECT_SOURCE_DIR}/etf/inc) # BINGER header path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_executable(pre pre.C)
target_link_libraries(pre libetf ${ROOT_LIBRARIES})

add_executable(sim sim.C)
target_link_libraries(sim libetf ${ROOT_LIBRARIES})

add_executable(eval eval.C)
target_link_libraries(eval libetf ${ROOT_LIBRARIES})

add_executable(ass assess.C)
target_link_libraries(ass libetf ${ROOT_LIBRARIES})

add_executable(t0 t0cali.C)
target_link_libraries(t0 libetf ${ROOT_LIBRARIES})

add_executable(str strcali.C)
target_link_libraries(str libetf ${ROOT_LIBRARIES})

add_executable(vme vme.C)
target_link_libraries(vme libetf ${ROOT_LIBRARIES})

add_executable(pid3 pid3.C)
target_link_libraries(pid3 libetf ${ROOT_LIBRARIES})

