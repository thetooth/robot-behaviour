cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

# Setting up ccache or sccache
find_program(CCACHE "ccache")
if(CCACHE)
    message("${CCACHE} has been found in the system!")
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
    find_program(SCCACHE "sccache")
    if(SCCACHE)
        message("${SCCACHE} has been found in the system!")
        set(CMAKE_C_COMPILER_LAUNCHER ${SCCACHE})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE})
    else()
        message("neither ccache or sccache have been found in the system!")
    endif(SCCACHE)
endif(CCACHE)

# Project
set(PROJECT_DESCRIPTION "BrainTree")
set(ONLINE_REPOSITORY "https://github.com/arvidsson/BrainTree")
project(
    "braintree"
        DESCRIPTION ${PROJECT_DESCRIPTION}
        HOMEPAGE_URL ${ONLINE_REPOSITORY}
)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)
# Clang compile commands are generated here:
# build/Debug/compile_commands.json
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/int")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)

target_link_libraries(
    ${PROJECT_NAME}
)
