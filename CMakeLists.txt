###########################
# CMake Build Environment #
###########################

# Project name
project(wannabebash)

# Minimum Cmake Version
cmake_minimum_required(VERSION 2.6)

#####################
# Setup Environment #
#####################

# set to include custom modules
set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} ${wannabebash_SOURCE_DIR}/cmake)

# set build type if specified by environment
if((NOT CMAKE_BUILD_TYPE) AND (NOT $ENV{CMAKE_BUILD_TYPE} STREQUAL ""))
    set(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
endif()

# Set include directories
include_directories(${wannabebash_SOURCE_DIR}/include)

# Get C files
file(GLOB SRC src/*c)

# Get executable files
file(GLOB EXECLIST bin/*c)



# determine build type
# 1) use build type if specified by the user.
# 2) if in-source building, use default (debug) build type.
# 3) if out-source building, determine based on the build directory's name which build type. 
#    If build directory is called 'Release' or 'Profile' then use the 'Release' and 'Profile' 
#    build type respectively, otherwise use 'debug'.
if(CMAKE_BUILD_TYPE)
  string(TOUPPER "${CMAKE_BUILD_TYPE}" TYPE)
  message("-- Using user specified build type: ${TYPE}")
elseif(${wannabebash_SOURCE_DIR} STREQUAL ${wannabebash_BINARY_DIR})
  set(CMAKE_BUILD_TYPE Debug)
  message("-- In-source building. Build type set to: Debug")
else()
  get_filename_component(TYPE ${wannabebash_BINARY_DIR} NAME)
  string(TOUPPER "${TYPE}" TYPE)
  if(${TYPE} STREQUAL "RELEASE")
    set(CMAKE_BUILD_TYPE Release)
  elseif(${TYPE} STREQUAL "PROFILE")
    set(CMAKE_BUILD_TYPE Profile)
  else()
    set(CMAKE_BUILD_TYPE Debug)
 endif()
 message("-- Out-source building. Build type set to: ${TYPE}")
endif()

# Handle non-default build type 'Profile'
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
if(${CMAKE_BUILD_TYPE} STREQUAL "PROFILE")
  set(CMAKE_BUILD_TYPE DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif()

#######################
# Set Compile Targets #
#######################
# src library (all C files in 'src' dir)
if(NOT SRC STREQUAL "")
  get_filename_component(LIBNAME ${wannabebash_SOURCE_DIR} NAME)
  set(LIBNAME "${LIBNAME}_core")
  add_library(${LIBNAME} ${SRC})
endif()

# executables (any C file in 'bin' dir)
foreach(EXEC ${EXECLIST})
  get_filename_component(EXECNAME ${EXEC} NAME_WE)
  add_executable(${EXECNAME} ${EXEC})

  if(NOT SRC STREQUAL "")
    target_link_libraries(${EXECNAME} LINK_PUBLIC ${LIBNAME})
  endif()

  message("-- Adding executable: ${EXECNAME}")
endforeach(EXEC)
