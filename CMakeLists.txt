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

cmake_minimum_required (VERSION 2.8)

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

add_subdirectory(src)
