# Install script for directory: /home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.3.2.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.3"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/build/OpenGLIncludes/assimp-3.3/code/libassimpd.so.3.2.0"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/build/OpenGLIncludes/assimp-3.3/code/libassimpd.so.3"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.3.2.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.3"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/build/OpenGLIncludes/assimp-3.3/code/libassimpd.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/anim.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/ai_assert.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/camera.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/color4.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/color4.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/config.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/defs.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/cfileio.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/light.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/material.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/material.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/matrix3x3.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/matrix3x3.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/matrix4x4.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/matrix4x4.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/mesh.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/postprocess.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/quaternion.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/quaternion.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/scene.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/metadata.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/texture.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/types.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/vector2.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/vector2.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/vector3.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/vector3.inl"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/version.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/cimport.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/importerdesc.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/Importer.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/DefaultLogger.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/ProgressHandler.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/IOStream.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/IOSystem.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/Logger.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/LogStream.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/NullLogger.hpp"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/cexport.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/Exporter.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/Compiler/pushpack1.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/Compiler/poppack1.h"
    "/home/allah/Desktop/Vscode-Workspace/Cpp/GLRenderer/OpenGLIncludes/assimp-3.3/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

