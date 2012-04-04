cmake_minimum_required (VERSION 2.8)

project(SOEM)

# La versió de SOEM. De moment la indico aquí i no la poso a les capçaleres, que ja la tenen posada
set (SOEM_VERSION_MAJOR 1)
set (SOEM_VERSION_MINOR 2)
set (SOEM_VERSION_PATCH 5)

add_subdirectory(src)
