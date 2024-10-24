cmake_minimum_required(VERSION 3.0)

project(FEM2D)

include_directories(${PROJECT_SOURCE_DIR}/..)

# find Boost
find_package(Boost 1.66.0 REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    # ignore BOOST deprecated headers
    add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
    add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")

    message("Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})
    message("Using Boost "
            ${Boost_MAJOR_VERSION} "."
            ${Boost_MINOR_VERSION} "."
            ${Boost_SUBMINOR_VERSION}
            " version"
    )
else ()
    message("Could not found Boost library!" FATAL_ERROR)
endif ()


set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

if ((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR CMAKE_COMPILER_IS_GNUCXX)
    # C++17 standard
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -std=c++17 -Wall -fpermissive -fPIC ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DTRILIBRARY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

    # sanitizer flags
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
    endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRILIBRARY -DNO_TIMER -D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCPU86")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif ()

find_package(PythonLibs REQUIRED)
find_package(Python2 REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPOLYMESH2D_EXPORTS -DTRIANGLEMESH_EXPORTS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTHRUST_DEVICE_SYSTEM=THRUST_DEVICE_SYSTEM_CPP")

message("CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message("CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message("CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG})
message("CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE})
message("CMAKE_CXX_FLAGS_RELWITHDEBINFO: " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
message("CMAKE_CXX_FLAGS_MINSIZEREL: " ${CMAKE_CXX_FLAGS_MINSIZEREL})


set(TRIANGLEMESH_SOURCES
        ${PROJECT_SOURCE_DIR}/mesh/trianglemesh/detail/trianglemesh.cpp
)

set(TRIANGLE_SOURCES
        ${PROJECT_SOURCE_DIR}/mesh/trianglemesh/detail/triangle/triangle.h
        ${PROJECT_SOURCE_DIR}/mesh/trianglemesh/detail/triangle/triangle.c
)

set(TARGET_EXE_NAME main)

set(TARGET_LIB_NAME FEM2D)

add_library(${TARGET_LIB_NAME}
        SHARED
        ${TARGET_EXE_NAME}
        ${TRIANGLEMESH_SOURCES}
        ${TRIANGLE_SOURCES}
)

include_directories(${PYTHON_INCLUDE_DIRS})

target_link_libraries(${TARGET_LIB_NAME} PRIVATE stdc++fs)

#executable stage
add_executable(${TARGET_EXE_NAME}
        main.cpp
        ${TRIANGLEMESH_SOURCES}
        ${TRIANGLE_SOURCES}
)

target_link_libraries(${TARGET_EXE_NAME} PRIVATE armadillo -lpython3.5m)
