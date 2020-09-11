cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
PROJECT(gltf_viewer)
include(CheckIncludeFiles)
include(FindPkgConfig)
include(CheckCXXSourceCompiles)

# build type
set(CMAKE_BUILD_TYPE Debug)

# compile option (Debug) 
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++17 -g -Wall -Wextra -Werror -O0 -pthread")
set(CMAKE_C_FLAGS_DEBUG "-std=c14 -g -Wall -Wextra -Werror -O0 -pthread")

# compile option (Release)
set(CMAKE_CXX_FLAGS_RELEASE "-march=native -std=c++17 -W -Wall -Wextra -Werror -O2 -pthread")
set(CMAKE_C_FLAGS_RELEASE "-march=native -std=c14 -W -Wall -Wextra -Werror -O2 -pthread")

#if you don't want the full compiler output, remove the following line
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(Threads REQUIRED)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.65.0 COMPONENTS filesystem program_options system REQUIRED )
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
find_package(GLFW REQUIRED)
find_package(Vulkan REQUIRED)
find_package(JSON REQUIRED)
find_package(FXGLTF REQUIRED)
find_package(OpenImageIO REQUIRED)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIR}
  ${JSON_INCLUDE_DIRS}
  ${FXGLTF_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIRS}
  ${OIIO_INCLUDE_DIR}
)
link_directories(
  ${Boost_LIBRARY_DIRS}
  ${SPDK_LIBRARY_DIRS}
  ${OIIO_LIBRARY_DIR}
)
subdirs( include src )

