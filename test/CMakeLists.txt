cmake_minimum_required (VERSION 3.10.0)
project (unit_test)

include_directories("${PROJECT_SOURCE_DIR}/../src")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SOURCES
    main.cpp
    ${PROJECT_SOURCE_DIR}/../src/tupp.cpp
    ${PROJECT_SOURCE_DIR}/../src/tupp.hpp
    )

add_executable (unit_test ${SOURCES} )
