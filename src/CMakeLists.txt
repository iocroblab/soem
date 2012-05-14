#########################################################################
# CMakeLists.txt file for building SOEM with CMake                      #
#                                                                       #
# Copyright (c) 2012 Orestes Mas                                        #
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

set(CMAKE_BUILD_TYPE Release)



include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# SOEM header files
set(SOEM_HDRS
  ethercatbase.h
  ethercatcoe.h
  ethercatconfig.h
  ethercatconfiglist.h
  ethercatdc.h
  ethercatfoe.h
  ethercatmain.h
  ethercatprint.h
  ethercatsoe.h
  ethercattype.h
  nicdrv.h
)

# SOEM source files (without examples)
set(SOEM_SRC
  ethercatbase.c
  ethercatconfig.c
  ethercatfoe.c
  ethercatprint.c
  eepromtool.c
  ethercatcoe.c
  ethercatdc.c
  ethercatmain.c
  ethercatsoe.c
  nicdrv.c
)

# Create static library (.a) with all basic SOEM functions
add_library(soem_static STATIC ${SOEM_SRC})
set_target_properties(soem_static 
  PROPERTIES DEFINE_SYMBOL "SOEM_DLL_EXPORT"
  OUTPUT_NAME soem
# Set accumulated compilation flags: (.so and .a)
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD}"
  LINK_FLAGS "${CMAKE_LD_FLAGS_ADD}"
  VERSION "${SOEM_VERSION}"
  CLEAN_DIRECT_OUTPUT 1)
# Create dynamic library (.so) with all basic SOEM functions
add_library(soem_dynamic SHARED ${SOEM_SRC})
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
  ENDIF ()



# Build example executables
# NOTE: The executables are statically linked (soem functions embedded)
# If you wish to link them dynamically with libsoem.so, 
# then change "soem_static" to "soem" in link commands below
add_executable(ebox ebox.c)
target_link_libraries(ebox soem_static ${CMAKE_THREAD_LIBS_INIT})

add_executable(simple_test simple_test.c)
target_link_libraries(simple_test soem_static)

add_executable(eepromtool eepromtool.c)
target_link_libraries(eepromtool soem_static)

add_executable(slaveinfo slaveinfo.c)
target_link_libraries(slaveinfo soem_static)

add_executable(red_test red_test.c)
target_link_libraries(red_test soem_static ${CMAKE_THREAD_LIBS_INIT})

# Install files into system directories
INSTALL(TARGETS ebox simple_test slaveinfo red_test eepromtool DESTINATION bin)
INSTALL(TARGETS soem_static soem_dynamic DESTINATION lib)
INSTALL(FILES ${SOEM_HDRS} DESTINATION include/soem)


if(ENABLE_RTNET)
  # Create static library (.a) with all basic SOEM functions
  include_directories(${RTNET_INCLUDE_DIR})
  
  add_library(soem_static_rt STATIC ${SOEM_SRC})
  set_target_properties(soem_static_rt 
    PROPERTIES DEFINE_SYMBOL "SOEM_DLL_EXPORT"
    OUTPUT_NAME soemrt
  # Set accumulated compilation flags: (.so and .a)
    COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RTNET} -DHAVE_RTNET"
    LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET} ${CMAKE_EXE_LINKER_FLAGS_RTNET}"
    VERSION "${SOEM_VERSION}"
    CLEAN_DIRECT_OUTPUT 1)
  # Create dynamic library (.so) with all basic SOEM functions
  add_library(soem_dynamic_rt SHARED ${SOEM_SRC})
  set_target_properties( soem_dynamic_rt PROPERTIES
    DEFINE_SYMBOL "SOEM_DLL_EXPORT"
    SOVERSION "${SOEM_VERSION_MAJOR}.${SOEM_VERSION_MINOR}.${SOEM_VERSION_PATCH}"
    VERSION "${SOEM_VERSION}"
    OUTPUT_NAME soemrt
  # Set accumulated compilation flags: (.so and .a)
    COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RTNET} -DHAVE_RTNET"
    LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET} ${CMAKE_EXE_LINKER_FLAGS_RTNET}"
    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    CLEAN_DIRECT_OUTPUT 1)
    IF (UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES( soem_dynamic PROPERTIES
      LINK_FLAGS "${LINK_FLAGS} -Wl,-zdefs")
    ENDIF ()

  # Build example executables
  # NOTE: The executables are statically linked (soem functions embedded)
  # If you wish to link them dynamically with libsoem.so, 
  # then change "soem_static" to "soem" in link commands below
  add_executable(ebox_rt ebox.c)
  target_link_libraries(ebox_rt soem_static_rt )
  set_target_properties(ebox_rt PROPERTIES
  LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET}" )

  add_executable(simple_test_rt simple_test.c)
  target_link_libraries(simple_test_rt soem_static_rt)

  add_executable(eepromtool_rt eepromtool.c)
  target_link_libraries(eepromtool_rt soem_static_rt)

  add_executable(slaveinfo_rt slaveinfo.c)
  target_link_libraries(slaveinfo_rt soem_static_rt)

  add_executable(red_test_rt red_test.c)
  target_link_libraries(red_test_rt soem_static_rt )
  set_target_properties(red_test_rt PROPERTIES
  LINK_FLAGS "${CMAKE_LD_FLAGS_ADD} ${CMAKE_SHARED_LINKER_FLAGS_RTNET}" )


  # Install files into system directories
  INSTALL(TARGETS ebox_rt simple_test_rt slaveinfo_rt red_test_rt eepromtool_rt DESTINATION bin)
  INSTALL(TARGETS soem_static_rt soem_dynamic_rt DESTINATION lib)
  #INSTALL(FILES ${SOEM_HDRS} DESTINATION include/soem)
endif()

### OPTIONAL ###
# CPack allows to automatically generate .deb (or whatever) package
# uncomment if useful
#SET(CPACK_GENERATOR "DEB")
#SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Leopold Palomo") #required
#SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SOEM library package")
#SET(CPACK_PACKAGE_VENDOR "IOC")
#SET(CPACK_PACKAGE_VERSION_MAJOR "1")
#SET(CPACK_PACKAGE_VERSION_MINOR "5")
#SET(CPACK_PACKAGE_VERSION_PATCH "2")

#include(CPack)

