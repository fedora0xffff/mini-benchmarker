cmake_minimum_required(VERSION 3.10)
project(mini-bench VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_VERSION 1)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_DESCRIPTION "A simple c++ benchmarker library")

add_executable(${PROJECT_NAME}_demo
    demo/demo.cpp
    src/benchmark.cpp
    src/bench_data.cpp
)

target_include_directories(${PROJECT_NAME}_demo PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME} SHARED
    src/benchmark.cpp
    src/bench_data.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

set(API include/minibench.h)
include(GNUInstallDirs)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER ${API} 
)

set(CONF_FILE "${PROJECT_NAME}.pc.in")
configure_file(${CONF_FILE} "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.pc" @ONLY)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.pc" 
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig"
)