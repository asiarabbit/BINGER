##################################################################################################
# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP
#
# CMakeLists.txt for user-composed programs.
#
# Created Time: 2017/10/18
# Author: SUN Yazhou, asia.rabbit@163.com
# Last Modified: 2018/9/19, SUN Yazhou
##################################################################################################

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS}) # ROOT header path
include_directories(${PROJECT_SOURCE_DIR}/etf/inc) # BINGER header path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIB_LIST libetf ${ROOT_LIBRARIES})

add_executable(pre pre.cxx)
target_link_libraries(pre ${LIB_LIST})

#add_executable(pre1 pre1.cxx)
#target_link_libraries(pre1 ${LIB_LIST})

add_executable(sim sim.cxx)
target_link_libraries(sim ${LIB_LIST})

#add_executable(eval eval.cxx)
#target_link_libraries(eval ${LIB_LIST})

add_executable(ass assess.cxx)
target_link_libraries(ass ${LIB_LIST})

add_executable(t0 t0cali.cxx)
target_link_libraries(t0 ${LIB_LIST})

add_executable(t0pdc t0cali_pdc.cxx)
target_link_libraries(t0pdc ${LIB_LIST})

add_executable(str strcali.cxx)
target_link_libraries(str ${LIB_LIST})

add_executable(strpdc strcali_pdc.cxx)
target_link_libraries(strpdc ${LIB_LIST})

add_executable(strdcta strcali_dcta.cxx)
target_link_libraries(strdcta ${LIB_LIST})

#add_executable(vme vme.cxx)
#target_link_libraries(vme ${LIB_LIST})

#add_executable(pid3 pid3.cxx)
#target_link_libraries(pid3 ${ROOT_LIBRARIES})

#add_executable(te test.cxx)
#target_link_libraries(te ${LIB_LIST})

add_executable(sho shoot.cxx)
target_link_libraries(sho ${LIB_LIST})

#add_executable(cut cut.cxx)
#target_link_libraries(cut ${LIB_LIST})

# build code for target - gui
#set(MG "MAMKE_GUI")
#add_custom_command(OUTPUT ${MG}
#	COMMAND make -C ${PROJECT_SOURCE_DIR}/src/gui
#	COMMAND cp ${PROJECT_SOURCE_DIR}/src/gui/gui ${PROJECT_BINARY_DIR}
#	COMMAND cp ${PROJECT_SOURCE_DIR}/src/gui/MyMainFrameDict_rdict.pcm ${PROJECT_BINARY_DIR}
#	COMMENT "Preparing for the buiding of gui"
#)
#add_custom_target(gui ALL DEPENDS ${MG})



