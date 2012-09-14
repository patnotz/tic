cmake_minimum_required(VERSION 2.8)

PROJECT(tic)
set(BUILD_SHARED_LIBS OFF)

################################################################################
# GTest - http://code.google.com/p/googletest/
################################################################################
add_subdirectory(gtest-1.6.0)
enable_testing(true)

################################################################################
# TBB
# See: http://code.google.com/p/findtbb/wiki/HowTo
################################################################################
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} .)
#find_package(TBB)
if("x${TIC_TBB_ROOT}" STREQUAL "x")
    # nothing
else("x${TIC_TBB_ROOT}" STREQUAL "x")
    set(TBB_INCLUDE_DIRS "${TIC_TBB_ROOT}/include")
    set(TBB_LIBRARY_DIRS "${TIC_TBB_ROOT}/lib")
    set(TBB_LIBRARIES "-ltbb")
    include_directories(${TBB_INCLUDE_DIRS})
    link_directories(${TBB_LIBRARY_DIRS})
endif("x${TIC_TBB_ROOT}" STREQUAL "x")

################################################################################
# Auto-version generation
################################################################################
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tic_autoversion.hpp
    COMMAND ${PROJECT_SOURCE_DIR}/create-version-header.sh ${PROJECT_SOURCE_DIR}/version-gen.sh ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS 
    COMMENT "Generating auto-version header file from Git..."
    ALL
)

################################################################################
# Uncrustify
################################################################################
add_custom_target(
    fmt
    COMMAND uncrustify -c ${PROJECT_SOURCE_DIR}/doc/google.cfg -l CPP --replace --mtime ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp
    DEPENDS 
    COMMENT "Uncrustifying source files"
)

################################################################################
# Boost
################################################################################
set(Boost_NO_SYSTEM_PATHS TRUE)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_ADDITIONAL_VERSIONS 1.47 1.47.0 1.48 1.48.0 1.49 1.49.0)
find_package(Boost 1.49 REQUIRED COMPONENTS filesystem system program_options chrono)

include_directories (
    src
    ${Boost_INCLUDE_DIRS}
    ${TBB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR} # for tic_version.hpp
)

link_directories (
    ${TBB_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
)

################################################################################
# The tic library
################################################################################
FILE(GLOB SOURCES "src/tic_*.cpp")
FILE(GLOB TIC_HEADERS "src/tic_*.hpp")
SET ( HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/tic_autoversion.hpp
    ${TIC_HEADERS}
)

add_library(ticlib ${SOURCES} ${HEADERS})

SET ( tic_LIBRARIES
    ticlib
)

#
# For each executable...
#
add_executable ( tic
    src/main_tic.cpp 
)
target_link_libraries ( tic
    ${tic_LIBRARIES}
    ${TBB_LIBRARIES} 
    ${Boost_LIBRARIES}
) 

install(TARGETS tic DESTINATION bin)

################################################################################
# Unit Tests
################################################################################
include_directories(src ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(unit-tests src/unit-tests.cpp)
target_link_libraries(unit-tests 
    gtest 
    gtest_main  
    ${tic_LIBRARIES} 
    ${TBB_LIBRARIES} 
    ${Boost_LIBRARIES})
add_test(unit-tests unit-tests)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS unit-tests)
