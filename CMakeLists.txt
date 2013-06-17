#########################################################################
# CMakeLists.txt file for building SOEM with CMake                      #
#                                                                       #
# Copyright (c) 2012 Orestes Mas <orestes@tsc.upc.edu>                  #
# Copyright (c) 2012-2013 Leopold Palomo-Avellaneda <leo@alaxarxa.net>  #
#                                                                       #
# This file is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <http://www.gnu.org/licenses/>. #
#########################################################################

cmake_minimum_required (VERSION 2.6)

project(SOEM)

# SOEM version. Defined here but not exported to header files, because it's already defined there 
set( SOEM_VERSION 1.3.0 )
string( REGEX MATCHALL "[0-9]+" SOEM_VERSIONS ${SOEM_VERSION} )
LIST( GET SOEM_VERSIONS 0 SOEM_VERSION_MAJOR)
LIST( GET SOEM_VERSIONS 1 SOEM_VERSION_MINOR)
LIST( GET SOEM_VERSIONS 2 SOEM_VERSION_PATCH)

# Some example programs use "libpthread"
# http://stackoverflow.com/questions/1620918/cmake-and-libpthread
find_package(Threads REQUIRED)

# Setting some flags
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
# Optional
#set (CMAKE_C_FLAGS "-fdump-rtl-expand")

#Check if we need to link against librt
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)
if(HAVE_CLOCK_GETTIME)
  set(CMAKE_SHARED_LINKER_FLAGS "-lrt ${CMAKE_SHARED_LINKER_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "-lrt ${CMAKE_EXE_LINKER_FLAGS}")
endif()

option(ENABLE_STATIC_LIB "Enable libsoem static." ON)
option(ENABLE_TESTS "Enable test programs and utilities linked dynamically." ON)
option(ENABLE_TESTS_STATIC "Enable test programs and utilities linked statically." OFF)

option(ENABLE_RTNET "Enable RTnet over Xenomai." ON)
option(ENABLE_YOUBOT_PATCH "Enable YouBot patch to solve a bug in the mailboxes." OFF)

if(ENABLE_YOUBOT_PATCH)
  add_definitions( -DUSE_YOUBOT )
endif()

if(ENABLE_RTNET)
  #common commands for building c++ executables and libraries
  find_path(RTNET_INCLUDE_DIR NAMES rtnet.h PATH_SUFFIXES rtnet/include)
  if(NOT RTNET_INCLUDE_DIR)
    set(ENABLE_RTNET OFF)
  endif()

  if(RTNET_INCLUDE_DIR)
    message("Found RTNET: ${RTNET_INCLUDE_DIR}")
    # try Xenomai
    # find Xenomai
    find_program(XENOCONFIG xeno-config PATHS /usr/xenomai/bin /usr/bin/ /usr/local/bin)
    message(STATUS "Xenomai found at: " ${XENOCONFIG})
    #execute_process(COMMAND ${XENOCONFIG} --skin=native --cflags OUTPUT_VARIABLE XENO_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    #execute_process(COMMAND ${XENOCONFIG} --skin=native --ldflags OUTPUT_VARIABLE XENO_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND ${XENOCONFIG} --skin=rtdm --cflags OUTPUT_VARIABLE XENO_CFLAGS_RTDM OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${XENOCONFIG} --skin=rtdm --ldflags OUTPUT_VARIABLE XENO_LDFLAGS_RTDM OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    # Needed for the examples
    execute_process(COMMAND ${XENOCONFIG} --skin=posix --cflags OUTPUT_VARIABLE XENO_CFLAGS_POSIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${XENOCONFIG} --skin=posix --ldflags OUTPUT_VARIABLE XENO_LDFLAGS_POSIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    

# setting some flags    
    set(XENO_CFLAGS_RTDM "-DRTNET ${XENO_CFLAGS_RTDM}")
    # for the examples
    set(XENO_CFLAGS_POSIX "-DRTNET ${XENO_CFLAGS_POSIX}")
    # We need to add the native lib
    set(CMAKE_LINKER_FLAGS_RTNET "-lnative ${XENO_LDFLAGS_RTDM}")

    message(STATUS "Compiler RTNET Flags for the library: " ${XENO_CFLAGS_RTDM})
    message(STATUS "Linker RTNET flags for the library: " ${CMAKE_LINKER_FLAGS_RTNET})
    
    message(STATUS "Compiler RTNET POSIX flags fotr the examples: " ${XENO_CFLAGS_POSIX})
    message(STATUS "Linker RTNET POSIX flags for the examples: " ${CMAKE_EXE_LINKER_FLAGS_RTNET})

  endif()
endif()

#set(CMAKE_BUILD_TYPE Release)

########################
# Defining the sources
########################

include_directories(${CMAKE_SOURCE_DIR}/osal)
include_directories(${CMAKE_SOURCE_DIR}/soem)

# SOEM header files
set(SOEM_HDRS
  soem/ethercatbase.h
  soem/ethercatcoe.h
  soem/ethercatconfig.h
  soem/ethercatconfiglist.h
  soem/ethercatdc.h
  soem/ethercatfoe.h
  soem/ethercatmain.h
  soem/ethercatprint.h
  soem/ethercatsoe.h
  soem/ethercattype.h
)

# SOEM source files (without examples)
set(SOEM_SRC
  soem/ethercatbase.c
  soem/ethercatcoe.c
  soem/ethercatconfig.c
  soem/ethercatdc.c
  soem/ethercatfoe.c
  soem/ethercatmain.c
  soem/ethercatprint.c
  soem/ethercatsoe.c
)

set(OSAL_H
     osal/osal.h  )

if(WIN32)
### OSAL source
  set(OSAL_SRC
     osal/win32/osal.c
  )
  set(OSAL_HDRS
     osal/win32/inttypes.h
     osal/win32/osal_win32.h
     osal/win32/stdint.h
  )
### OSWL source
### the pcap part is not included ...
### it doesn't works ....
  include_directories(${CMAKE_SOURCE_DIR}/oshw/win32)
  set(OSHW_SRC
     oshw/win32/nicdrv.c
     oshw/win32/oshw.c
  )
  set(OSHW_HDRS
     oshw/win32/oshw.h
     oshw/win32/nicdrv.h
  )
endif(WIN32)

if(UNIX)
### OSAL source
  set(OSAL_SRC
     osal/linux/osal.c
  )
  set(OSAL_HDRS
     osal/linux/inttypes.h
     osal/linux/osal_win32.h
     osal/linux/stdint.h
  )
  include_directories(${CMAKE_SOURCE_DIR}/oshw/linux)
### OSWL source
  set(OSHW_SRC
     oshw/linux/nicdrv.c
     oshw/linux/oshw.c
  )
  set(OSHW_HDRS
     oshw/linux/oshw.h
     oshw/linux/nicdrv.h
  )
endif(UNIX)

# Create static library (.a) with all basic SOEM functions

if(ENABLE_STATIC_LIB)
  add_library(soem_static STATIC ${SOEM_SRC} ${OSHW_SRC} ${OSAL_SRC})
  set_target_properties(soem_static PROPERTIES 
    OUTPUT_NAME soem
# Set accumulated compilation flags: (.so and .a)
#  COMPILE_FLAGS "${CMAKE_C_FLAGS}"
#  LINK_FLAGS "${CMAKE_LD_FLAGS}"
   INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
   CLEAN_DIRECT_OUTPUT 1)
endif()

# Create dynamic library (.so) with all basic SOEM functions
add_library(soem_dynamic SHARED ${SOEM_SRC} ${OSHW_SRC} ${OSAL_SRC})
set_target_properties( soem_dynamic PROPERTIES
  DEFINE_SYMBOL "SOEM_DLL_EXPORT"
  SOVERSION "${SOEM_VERSION_MAJOR}.${SOEM_VERSION_MINOR}.${SOEM_VERSION_PATCH}"
  VERSION "${SOEM_VERSION}"
  OUTPUT_NAME soem
# Set accumulated compilation flags: (.so and .a)
#  COMPILE_FLAGS "${XENO_CFLAGS_RTDM}"
  LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}"
  INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
  CLEAN_DIRECT_OUTPUT 1)

# # Build example executables
# # NOTE: The executables are dynamically linked 

if(ENABLE_TESTS)
#ebox eepromtool red_test simple_test slaveinfo firm_update
  if(WIN32)
    add_executable(ebox test/win32/ebox/ebox.c)
    target_link_libraries(ebox soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
 
    add_executable(simple_test test/win32/simple_test/simple_test.c)
    target_link_libraries(simple_test soem_dynamic)
 
    add_executable(eepromtool test/win32/eepromtool/eepromtool.c)
    target_link_libraries(eepromtool soem_dynamic)
 
    add_executable(slaveinfo test/win32/slaveinfo/slaveinfo.c)
    target_link_libraries(slaveinfo soem_dynamic)
 
    add_executable(red_test test/win32/red_test/red_test.c)
    target_link_libraries(red_test soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
    
    add_executable(firm_update test/win32/firm_update/firm_update.c)
    target_link_libraries(firm_update soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
  endif()

  if(UNIX)
    add_executable(ebox test/linux/ebox/ebox.c)
    target_link_libraries(ebox soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
 
    add_executable(simple_test test/linux/simple_test/simple_test.c)
    target_link_libraries(simple_test soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
 
    add_executable(eepromtool test/linux/eepromtool/eepromtool.c)
    target_link_libraries(eepromtool soem_dynamic)
 
    add_executable(slaveinfo test/linux/slaveinfo/slaveinfo.c)
    target_link_libraries(slaveinfo soem_dynamic)
 
    add_executable(red_test test/linux/red_test/red_test.c)
    target_link_libraries(red_test soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
    
    add_executable(firm_update test/linux/firm_update/firm_update.c)
    target_link_libraries(firm_update soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
  endif()
# Installing
  INSTALL(TARGETS ebox eepromtool red_test simple_test slaveinfo firm_update DESTINATION bin)

endif(ENABLE_TESTS)


if(ENABLE_STATIC_LIB)
  INSTALL(TARGETS soem_static DESTINATION lib)
endif()

# Defaul install
INSTALL(TARGETS soem_dynamic DESTINATION lib)
# Just ones headers
INSTALL(FILES ${SOEM_HDRS} ${OSAL_H} ${OSHW_HDRS} DESTINATION include/soem)

# Static tests
if(ENABLE_TESTS_STATIC)
  set(CMAKE_EXE_LINKER_FLAGS_STATIC "${CMAKE_EXE_LINKER_FLAGS} -static")
  add_executable(ebox_s test/linux/ebox/ebox.c)
  target_link_libraries(ebox_s soem_static ${CMAKE_EXE_LINKER_FLAGS_STATIC} ${CMAKE_THREAD_LIBS_INIT})
 
  add_executable(simple_test_s test/linux/simple_test/simple_test.c)
  target_link_libraries(simple_test_s soem_static ${CMAKE_EXE_LINKER_FLAGS_STATIC} ${CMAKE_THREAD_LIBS_INIT})
 
  add_executable(eepromtool_s test/linux/eepromtool/eepromtool.c)
  target_link_libraries(eepromtool_s soem_static ${CMAKE_EXE_LINKER_FLAGS_STATIC} ${CMAKE_THREAD_LIBS_INIT})
 
  add_executable(slaveinfo_s test/linux/slaveinfo/slaveinfo.c)
  target_link_libraries(slaveinfo_s soem_static ${CMAKE_EXE_LINKER_FLAGS_STATIC} ${CMAKE_THREAD_LIBS_INIT})
  
  add_executable(red_test_s test/linux/red_test/red_test.c)
  target_link_libraries(red_test_s soem_static ${CMAKE_EXE_LINKER_FLAGS_STATIC} ${CMAKE_THREAD_LIBS_INIT})
    
  add_executable(firm_update_s test/linux/firm_update/firm_update.c)
  target_link_libraries(firm_update_s soem_static ${CMAKE_EXE_LINKER_FLAGS_STATIC} ${CMAKE_THREAD_LIBS_INIT})

# Installing
  INSTALL(TARGETS ebox_s eepromtool_s red_test_s simple_test_s slaveinfo_s firm_update_s DESTINATION bin)

endif()

# RTNET part
###############################################################################  
if(ENABLE_RTNET)

  # Create static library (.a) with all basic SOEM functions
  include_directories(${RTNET_INCLUDE_DIR})

  if(ENABLE_STATIC_LIB)
    add_library(soem_static_rt STATIC ${SOEM_SRC} ${OSHW_SRC} ${OSAL_SRC})

  set_target_properties(soem_static_rt 
    OUTPUT_NAME soemrt 
    COMPILE_FLAGS "${XENO_CFLAGS_RTDM}"
    LINK_FLAGS "${CMAKE_LINKER_FLAGS_RTNET}"
    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    CLEAN_DIRECT_OUTPUT 1)
  endif()

  # Create dynamic library (.so) with all basic SOEM functions
  add_library(soem_dynamic_rt SHARED ${SOEM_SRC} ${OSHW_SRC} ${OSAL_SRC})
  set_target_properties( soem_dynamic_rt PROPERTIES
    DEFINE_SYMBOL "SOEM_DLL_EXPORT"
    SOVERSION "${SOEM_VERSION_MAJOR}.${SOEM_VERSION_MINOR}.${SOEM_VERSION_PATCH}"
    VERSION "${SOEM_VERSION}"
    OUTPUT_NAME soemrt
    COMPILE_FLAGS "${XENO_CFLAGS_RTDM}"
    LINK_FLAGS "${CMAKE_LINKER_FLAGS_RTNET}"
    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    CLEAN_DIRECT_OUTPUT 1)

  if(ENABLE_TESTS)
    add_executable(ebox_rt test/linux/ebox/ebox.c)
    target_link_libraries(ebox_rt soem_dynamic_rt )
    set_target_properties(ebox_rt PROPERTIES
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET}")
 
    add_executable(simple_test_rt test/linux/simple_test/simple_test.c)
    target_link_libraries(simple_test_rt soem_dynamic_rt)
    set_target_properties(simple_test_rt PROPERTIES
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET}")
 
    add_executable(eepromtool_rt test/linux/eepromtool/eepromtool.c)
    target_link_libraries(eepromtool_rt soem_dynamic_rt)
    set_target_properties(eepromtool_rt PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET}")
 
    add_executable(slaveinfo_rt test/linux/slaveinfo/slaveinfo.c)
    target_link_libraries(slaveinfo_rt soem_dynamic_rt)
    set_target_properties(slaveinfo_rt PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET}")

    add_executable(red_test_rt test/linux/red_test/red_test.c)
    target_link_libraries(red_test_rt soem_dynamic_rt)
    set_target_properties(red_test_rt PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET}")
    
    add_executable(firm_update_rt test/linux/firm_update/firm_update.c)
    target_link_libraries(firm_update_rt soem_dynamic_rt)
    set_target_properties(firm_update_rt PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET}")

    INSTALL(TARGETS ebox_rt eepromtool_rt red_test_rt simple_test_rt slaveinfo_rt firm_update_rt 
          DESTINATION bin)
  endif()

 if(ENABLE_TESTS_STATIC)
    set(CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC "${CMAKE_EXE_LINKER_FLAGS_RTNET} -static")

    add_executable(ebox_rt test/linux/ebox/ebox.c)
    target_link_libraries(ebox_rt_s soem_static_rt )
    set_target_properties(ebox_rt_s PROPERTIES
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC}")
 
    add_executable(simple_test_rt_s test/linux/simple_test/simple_test.c)
    target_link_libraries(simple_test_rt_s soem_static_rt)
    set_target_properties(simple_test_rt_s PROPERTIES
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC}")
 
    add_executable(eepromtool_rt_s test/linux/eepromtool/eepromtool.c)
    target_link_libraries(eepromtool_rt_s soem_static_rt)
    set_target_properties(eepromtool_rt_s PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC}")
 
    add_executable(slaveinfo_rt_s test/linux/slaveinfo/slaveinfo.c)
    target_link_libraries(slaveinfo_rt_s soem_static_rt)
    set_target_properties(slaveinfo_rt_s PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC}")

    add_executable(red_test_rt_s test/linux/red_test/red_test.c)
    target_link_libraries(red_test_rt_s soem_static_rt)
    set_target_properties(red_test_rt_s PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC}")
    
    add_executable(firm_update_rt_s test/linux/firm_update/firm_update.c)
    target_link_libraries(firm_update_rt_s soem_static_rt)
    set_target_properties(firm_update_rt_s PROPERTIES       
      COMPILE_FLAGS "${XENO_CFLAGS_POSIX}"
      LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RTNET_STATIC}")
    
    INSTALL(TARGETS ebox_rt_s eepromtool_rt_s red_test_rt_s simple_test_rt_s slaveinfo_rt_s firm_update_rt_s
          DESTINATION bin)
  endif()

  # Install files into system directories
  if(ENABLE_TESTS)
    
  endif()
  if(ENABLE_TESTS_STATIC)
    
  endif()
endif()

