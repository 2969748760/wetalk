# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-src"
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-build"
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix"
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/tmp"
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp"
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src"
  "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/QtProjects/GateServer/cmake-build-debug/_deps/boost-subbuild/boost-populate-prefix/src/boost-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
