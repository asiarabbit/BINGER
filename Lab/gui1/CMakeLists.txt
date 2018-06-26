PROJECT(gui)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

    set(LOG "log.txt")  
    add_custom_command(OUTPUT  ${LOG}  
      COMMAND echo "Generating log.txt file..."  
      COMMAND echo "welcome"  
      COMMENT  "This is a test"  
     )  
    add_custom_target(T1 ALL DEPENDS ${LOG})  
