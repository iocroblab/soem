###################################################################
# Fitxer CMakeLists.txt per construir la llibreria SOEM amb CMake #
# Orestes Mas, 27/2/2012                                          #
###################################################################

set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Alguns exemples fan ús de la llibreria "pthread"
# http://stackoverflow.com/questions/1620918/cmake-and-libpthread
find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# fitxers font del SOEM (sense els exemples)
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

# Creem la llibreria estàtica amb totes les funcions bàsiques del SOEM
add_library(soem_st STATIC ${SOEM_SRC})
# Creem la llibreria dinàmica amb totes les funcions bàsiques del SOEM
add_library(soem SHARED ${SOEM_SRC})

# Ara construïm els executables d'exemple
add_executable(ebox ebox.c)
target_link_libraries(ebox soem ${CMAKE_THREAD_LIBS_INIT})

add_executable(simple_test simple_test.c)
target_link_libraries(simple_test soem_st)

add_executable(eepromtool eepromtool.c)
target_link_libraries(eepromtool soem_st)

add_executable(slaveinfo slaveinfo.c)
target_link_libraries(slaveinfo soem_st)

add_executable(red_test red_test.c)
target_link_libraries(red_test soem_st ${CMAKE_THREAD_LIBS_INIT})

# Falta instal·lar la llibreria en algun directori del sistema
INSTALL(TARGETS soem DESTINATION lib) # a /usr/lib

# El CPack permet generar el paquet debian o el que sigui
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Leopold Palomo") #required
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64") # O és x86_64?
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SOEM library package")
SET(CPACK_PACKAGE_VENDOR "IOC")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "5")
SET(CPACK_PACKAGE_VERSION_PATCH "2")

include(CPack)

