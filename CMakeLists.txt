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

cmake_minimum_required (VERSION 2.6)

project(SOEM)

# SOEM version. Defined here but not exported to header files, because it's already defined there 
set( SOEM_VERSION 1.2.5 )
string( REGEX MATCHALL "[0-9]+" SOEM_VERSIONS ${SOEM_VERSION} )
LIST( GET SOEM_VERSIONS 0 SOEM_VERSION_MAJOR)
LIST( GET SOEM_VERSIONS 1 SOEM_VERSION_MINOR)
LIST( GET SOEM_VERSIONS 2 SOEM_VERSION_PATCH)
# Some example programs use "libpthread"
# http://stackoverflow.com/questions/1620918/cmake-and-libpthread
find_package(Threads REQUIRED)

set (CMAKE_C_FLAGS "-fdump-rtl-expand")

option(ENABLE_RTNET "Enable RTnet over Xenomai." OFF)

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

add_subdirectory(src)