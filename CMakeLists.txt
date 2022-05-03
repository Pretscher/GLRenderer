
cmake_minimum_required(VERSION 3.0.0)
project(GLRenderer VERSION 0.1.0)

file(COPY fileRecources DESTINATION .)

file(GLOB allFiles
     "src/*.hpp"
     "src/*.cpp"
     "src/*.h"
     "src/*.c"
)
add_executable(GLRenderer ${allFiles})


include_directories(GLRenderer ${PARENTDIR})
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(GLRenderer glfw ${GLFW_LIBRARIES})

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(GLRenderer ${ASSIMP_LIBRARIES} -pthread ${CMAKE_DL_LIBS})
add_subdirectory(src)

