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

set (CMAKE_C_FLAGS "-fdump-rtl-expand")

option(ENABLE_RTNET "Enable RTnet over Xenomai." OFF)
option(ENABLE_TESTS "Enable test programs and utilities." ON)

if(ENABLE_RTNET)
  #common commands for building c++ executables and libraries
  find_path(RTNET_INCLUDE_DIR NAMES rtnet.h PATH_SUFFIXES rtnet/include)
  if(NOT RTNET_INCLUDE_DIR)
    set(ENABLE_RTNET OFF)
  endif()

  if(RTNET_INCLUDE_DIR)
    message("Found RTNET: ${RTNET_INCLUDE_DIR}")
    # try Xenomai
    execute_process(COMMAND xeno-config --skin=posix --cflags OUTPUT_VARIABLE XENO_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND xeno-config --skin=posix --ldflags OUTPUT_VARIABLE XENO_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_C_FLAGS_RTNET "${XENO_CFLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RTNET "${XENO_LDFLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RTNET "${XENO_LDFLAGS} -lnative")
  endif()
endif()

set(CMAKE_BUILD_TYPE Release)

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
add_library(soem_static STATIC ${SOEM_SRC} ${OSHW_SRC} ${OSAL_SRC})
set_target_properties(soem_static 
  PROPERTIES DEFINE_SYMBOL "SOEM_DLL_EXPORT"
  OUTPUT_NAME soem
# Set accumulated compilation flags: (.so and .a)
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD}"
  LINK_FLAGS "${CMAKE_LD_FLAGS_ADD}"
  VERSION "${SOEM_VERSION}"
  CLEAN_DIRECT_OUTPUT 1)
  if(UNIX AND NOT APPLE)
    target_link_libraries(soem_static rt)
  endif()
# Create dynamic library (.so) with all basic SOEM functions
add_library(soem_dynamic SHARED ${SOEM_SRC} ${OSHW_SRC} ${OSAL_SRC})
set_target_properties( soem_dynamic PROPERTIES
  DEFINE_SYMBOL "SOEM_DLL_EXPORT"
  SOVERSION "${SOEM_VERSION_MAJOR}.${SOEM_VERSION_MINOR}.${SOEM_VERSION_PATCH}"
  VERSION "${SOEM_VERSION}"
  OUTPUT_NAME soem
# Set accumulated compilation flags: (.so and .a)
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD}"
  LINK_FLAGS "${CMAKE_LD_FLAGS_ADD}"
  INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
  CLEAN_DIRECT_OUTPUT 1)
  IF (UNIX AND NOT APPLE)
   SET_TARGET_PROPERTIES( soem_dynamic PROPERTIES
	  LINK_FLAGS "-Wl,-zdefs")
   target_link_libraries(soem_dynamic rt)
  endif ()

# # Build example executables
# # NOTE: The executables are statically linked (soem functions embedded)
# # If you wish to link them dynamically with libsoem.so, 
# # then change "soem_static" to "soem" in link commands below

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
    target_link_libraries(simple_test soem_dynamic)
 
    add_executable(eepromtool test/linux/eepromtool/eepromtool.c)
    target_link_libraries(eepromtool soem_dynamic)
 
    add_executable(slaveinfo test/linux/slaveinfo/slaveinfo.c)
    target_link_libraries(slaveinfo soem_dynamic)
 
    add_executable(red_test test/linux/red_test/red_test.c)
    target_link_libraries(red_test soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
    
    add_executable(firm_update test/linux/firm_update/firm_update.c)
    target_link_libraries(firm_update soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
  endif()

endif(ENABLE_TESTS)

##############################################################################
# TODO 
# INSTALL targets
##############################################################################
# add_executable(ebox ebox.c)
# target_link_libraries(ebox soem_dynamic ${CMAKE_THREAD_LIBS_INIT})
# 
# add_executable(simple_test simple_test.c)
# target_link_libraries(simple_test soem_static)
# 
# add_executable(eepromtool eepromtool.c)
# target_link_libraries(eepromtool soem_static)
# 
# add_executable(slaveinfo slaveinfo.c)
# target_link_libraries(slaveinfo soem_static)
# 
# add_executable(red_test red_test.c)
# target_link_libraries(red_test soem_static ${CMAKE_THREAD_LIBS_INIT})
# 
# Install files into system directories
INSTALL(TARGETS ebox eepromtool red_test simple_test slaveinfo firm_update DESTINATION bin)
INSTALL(TARGETS soem_static soem_dynamic DESTINATION lib)
INSTALL(FILES ${SOEM_HDRS} ${OSAL_H} ${OSHW_HDRS} DESTINATION include/soem)

 
# # Build example executables
# # NOTE: The executables are statically linked (soem functions embedded)
# # If you wish to link them dynamically with libsoem.so, 
# # then change "soem_static" to "soem" in link commands below
# 
# 
# if(ENABLE_RTNET)
#   # Create static library (.a) with all basic SOEM functions
#   include_directories(${RTNET_INCLUDE_DIR})
#   
#   add_library(soem_static_rt STATIC ${SOEM_SRC})
#   set_target_properties(soem_static_rt 
#     PROPERTIES DEFINE_SYMBOL "SOEM_DLL_EXPORT"
#     OUTPUT_NAME soemrt
#   # Set accumulated compilation flags: (.so and .a)
#     COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RTNET} -DHAVE_RTNET"
#     LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET} ${CMAKE_EXE_LINKER_FLAGS_RTNET}"
#     VERSION "${SOEM_VERSION}"
#     CLEAN_DIRECT_OUTPUT 1)
#   # Create dynamic library (.so) with all basic SOEM functions
#   add_library(soem_dynamic_rt SHARED ${SOEM_SRC})
#   set_target_properties( soem_dynamic_rt PROPERTIES
#     DEFINE_SYMBOL "SOEM_DLL_EXPORT"
#     SOVERSION "${SOEM_VERSION_MAJOR}.${SOEM_VERSION_MINOR}.${SOEM_VERSION_PATCH}"
#     VERSION "${SOEM_VERSION}"
#     OUTPUT_NAME soemrt
#   # Set accumulated compilation flags: (.so and .a)
#     COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RTNET} -DHAVE_RTNET"
#     LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET} ${CMAKE_EXE_LINKER_FLAGS_RTNET}"
#     INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
#     CLEAN_DIRECT_OUTPUT 1)
#     IF (UNIX AND NOT APPLE)
#     SET_TARGET_PROPERTIES( soem_dynamic PROPERTIES
#       LINK_FLAGS "${LINK_FLAGS} -Wl,-zdefs")
#     ENDIF ()
# 
#   # Build example executables
#   # NOTE: The executables are statically linked (soem functions embedded)
#   # If you wish to link them dynamically with libsoem.so, 
#   # then change "soem_static" to "soem" in link commands below
#   add_executable(ebox_rt ebox.c)
#   target_link_libraries(ebox_rt soem_static_rt )
#   set_target_properties(ebox_rt PROPERTIES
#   LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET}" )
# 
#   add_executable(simple_test_rt simple_test.c)
#   target_link_libraries(simple_test_rt soem_static_rt)
# 
#   add_executable(eepromtool_rt eepromtool.c)
#   target_link_libraries(eepromtool_rt soem_static_rt)
# 
#   add_executable(slaveinfo_rt slaveinfo.c)
#   target_link_libraries(slaveinfo_rt soem_static_rt)
# 
#   add_executable(red_test_rt red_test.c)
#   target_link_libraries(red_test_rt soem_static_rt )
#   set_target_properties(red_test_rt PROPERTIES
#   LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET}" )
# 
# 
#   # Install files into system directories
#   INSTALL(TARGETS ebox_rt simple_test_rt slaveinfo_rt red_test_rt eepromtool_rt DESTINATION bin)
#   INSTALL(TARGETS soem_static_rt soem_dynamic_rt DESTINATION lib)
#   #INSTALL(FILES ${SOEM_HDRS} DESTINATION include/soem)
#endif()

