# make a custom gVirtualXRay configuration file

# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (I.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11

SET (USE_SUPERBUILD ON)
INCLUDE(ExternalProject)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (gVirtualXRay_INSTALL_DIR ${CMAKE_BINARY_DIR}/install)

ExternalProject_Add(gVirtualXRay
  SVN_REPOSITORY "https://svn.code.sf.net/p/gvirtualxray/code/trunk/gvxr/"
  SVN_TRUST_CERT TRUE
  UPDATE_COMMAND ""
  SOURCE_DIR  source
  BINARY_DIR  build
  INSTALL_DIR ${gVirtualXRay_INSTALL_DIR}
  LIST_SEPARATOR :::
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
      #-DgVirtualXRay_BUILD_TESTS:BOOL=OFF
      #-DBUILD_TESTING:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DCMAKE_INSTALL_PREFIX:PATH=${gVirtualXRay_INSTALL_DIR}
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
      #-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      #-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DgVirtualXRay_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
)

# copy libraries into install tree
install(DIRECTORY ${gVirtualXRay_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${gVirtualXRay_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
#install(DIRECTORY ${gVirtualXRay_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${gVirtualXRay_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
#install(DIRECTORY ${gVirtualXRay_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
#install(DIRECTORY ${gVirtualXRay_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")
