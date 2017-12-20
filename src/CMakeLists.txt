##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2017/10/18
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2017/10/21, SUN Yazhou
##################################################################################################

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS}) # header path
include_directories(${PROJECT_SOURCE_DIR}/etf/inc) # header path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_executable(pre pre.C)
target_link_libraries(pre ${ROOT_LIBRARIES} libetf)

add_executable(sim sim.C)
target_link_libraries(sim ${ROOT_LIBRARIES} libetf)

add_executable(eval eval.C)
target_link_libraries(eval ${ROOT_LIBRARIES} libetf)

add_executable(ass assess.C)
target_link_libraries(ass ${ROOT_LIBRARIES} libetf)

