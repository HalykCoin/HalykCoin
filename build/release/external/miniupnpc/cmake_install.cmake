# Install script for directory: /home/oleg/Projects/halykcoin/external/miniupnpc

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
    set(CMAKE_INSTALL_CONFIG_NAME "release")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/oleg/Projects/halykcoin/build/release/external/miniupnpc/libminiupnpc.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/miniupnpc" TYPE FILE FILES
    "/home/oleg/Projects/halykcoin/external/miniupnpc/miniupnpc.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/miniwget.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/upnpcommands.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/igd_desc_parse.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/upnpreplyparse.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/upnperrors.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/upnpdev.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/miniupnpctypes.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/portlistingparse.h"
    "/home/oleg/Projects/halykcoin/external/miniupnpc/miniupnpc_declspec.h"
    )
endif()

