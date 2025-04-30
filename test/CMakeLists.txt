cmake_minimum_required(VERSION 3.29)

set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")

set(PROJECT_NAME test_cplm)
project(${PROJECT_NAME})

enable_language(CUDA)

set(CPLM_HEADERS ${HEADERS})
set(CPLM_SOURCES ${SOURCES})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HEADERS "catch_amalgamated.hpp")
set(SOURCES
    ${SOURCE_DIR}/catch_amalgamated.cpp
    ${SOURCE_DIR}/test_cplm.cpp
    ${SOURCE_DIR}/main.cpp)

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR}../include)

source_group("include" FILES ${HEADERS} ${CPLM_HEADERS})
source_group("src" FILES ${SOURCES} ${CPLM_SOURCES} ${CUDA_SOURCES})

set(FILES ${HEADERS} ${SOURCES} ${CPLM_HEADERS} ${CPLM_SOURCES})

add_executable(${PROJECT_NAME} ${FILES} ${CUDA_SOURCES})
#CUDA_WRAP_SRCS(${PROJECT_NAME} OBJ generated_files infer.cu)

if(MSVC)
    set(DEFAULT_CXX_FLAGS "/DWIN32 /D_WINDOWS /D_UNICODE /DUNICODE /W4 /WX- /nologo /fp:precise /arch:AVX2 /Zc:wchar_t /TP /Gd /std:c++20 /std:c11 /utf-8 /openmp")
    if(MSVC_VERSION VERSION_LESS_EQUAL "1900")
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /Zc:__cplusplus /std:c++latest")
    else()
        set(DEFAULT_CXX_FLAGS "${DEFAULT_CXX_FLAGS} /Zc:__cplusplus /std:c++20")
    endif()
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1 /Gy /GR- /GS /Gm- /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Oi /GL /GR- /DNDEBUG /EHsc-")
    target_link_libraries(${PROJECT_NAME} MIMALLOC CUDA)

elseif(UNIX)
    set(DEFAULT_CXX_FLAGS "-Wall -O2 -std=c++20 -std=gnu++20 -march=x86-64-v3 -mavx2 -fno-exceptions -DVK_USE_PLATFORM_WIN32_KHR")
    set(CMAKE_CXX_FLAGS "${DEFAULT_CXX_FLAGS}")
    target_link_libraries(${PROJECT_NAME} MIMALLOC)
elseif(APPLE)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_ARCHITECTURES "80;90")
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME_DEBUG "${PROJECT_NAME}" OUTPUT_NAME_RELEASE "${PROJECT_NAME}"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_dependencies(${PROJECT_NAME} cplm)

