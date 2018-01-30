#  :copyright: (c) 2017-2018 Alex Huszagh.
#  :license: MIT, see licenses/mit.md for more details.

cmake_minimum_required(VERSION 3.1.0)
project(contained CXX)
enable_testing()

# OPTIONS
option(CONTAINED_BUILD_TESTS "Build contained library tests" OFF)

# STATIC/SHARED
if(NOT BUILD_SHARED_LIBS)
    set(BUILD_STATIC_LIBS OFF)
    if(MINGW OR MSYS OR CYGWIN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
endif()

set(CONTAINED_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/contained/deque.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contained/forward_list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contained/list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contained/string.h
    ${CMAKE_CURRENT_SOURCE_DIR}/contained/vector.h
)

set(CONTAINED_TESTS
    test/deque.cc
    test/forward_list.cc
    test/list.cc
    test/string.cc
    test/vector.cc
)

add_library(contained INTERFACE)
target_sources(contained INTERFACE ${CONTAINED_HEADERS})
target_include_directories(contained INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(CONTAINED_BUILD_TESTS)
    include(GoogleTest)
    add_executable(contained_tests ${CONTAINED_TESTS})
    target_link_libraries(contained_tests
        contained
        gtest
        gtest_main
    )
    add_test(NAME contained_tests COMMAND contained_tests)
endif()

# INSTALL
install(DIRECTORY contained
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# UNINSTALL
if(NOT TARGET uninstall)
    set(UNINSTALL_FILE ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
    set(UNINSTALL_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in)
    configure_file("${UNINSTALL_FILE_IN}" "${UNINSTALL_FILE}" IMMEDIATE @ONLY)

    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${UNINSTALL_FILE})
endif()
