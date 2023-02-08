cmake_minimum_required(VERSION 2.8)
project(ClawnTest)
include(${PROJECT_SOURCE_DIR}/../cmake/DownloadProject/DownloadProject.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_custom_target(build_test)

download_project(
    PROJ googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++17 --debug -O0")
set(SRC_DIR ${PROJECT_SOURCE_DIR}/../src)

find_package(LLVM 9)
find_package(bison 3.5)
find_package(FLEX 2.6.4)

set(FLEX_INCLUDE_DIRS /usr/local/opt/flex/include)
set(FLEX_LIBRARIES /usr/local/opt/flex/lib)
bison_target(parser ${SRC_DIR}/lib/clawn/parser/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
flex_target(lexer ${SRC_DIR}/lib/clawn/parser/lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)
llvm_map_components_to_libnames(llvm_libs support core irreader) 


set(ld_flags " -std=c++17 --debug -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags} ")
file(GLOB_RECURSE SRC_CPP_FILES ${PROJECT_SOURCE_DIR}/../src/lib/**/*.cpp)
file(GLOB_RECURSE SRC_HPP_FILES ${PROJECT_SOURCE_DIR}/../src/include/**/*.hpp)
file(GLOB_RECURSE TEST_TARGET_CPP_FILES ${PROJECT_SOURCE_DIR}/unittests/**/*.cpp)
list(REMOVE_ITEM SRC_CPP_FILES ${PROJECT_SOURCE_DIR}/src/lib/mir/*.*)
list(REMOVE_ITEM SRC_CPP_FILES ${PROJECT_SOURCE_DIR}../src/lib/clawn/main.cpp)

execute_process(COMMAND llvm-config --libdir OUTPUT_VARIABLE LLVM_LIBS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --cxxflags --ldflags  --system-libs --libs all OUTPUT_VARIABLE LLVM_OPTIONS)
string(REPLACE "\n" " " LLVM_OPTIONS ${LLVM_OPTIONS})
string(REPLACE "-fno-exceptions" " " LLVM_OPTIONS ${LLVM_OPTIONS})
string(REPLACE "-std=c++14" " " LLVM_OPTIONS ${LLVM_OPTIONS})

set(ld_flags "--debug -O0 -Wno-unused-command-line-argument -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}")
set(CMAKE_CXX_FLAGS "-std=c++17 --debug -O0 -Wno-unused-command-line-argument ${LLVM_OPTIONS}")

#foreach(filename IN LISTS TEST_TARGET_CPP_FILES)
#    message(${filename})
#endforeach()


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${CMAKE_CURRENT_BINARY_DIR} ${SRC_DIR} ${GTEST_DIR} ${FLEX_INCLUDE_DIRS} /usr/local/opt/flex/include /usr/local/Cellar/llvm/9.0.0_1/include)
add_executable(clawn_test
    ${PROJECT_SOURCE_DIR}/test_main.cpp
    ${TEST_TARGET_CPP_FILES}
    )
target_link_libraries(clawn_test Parser Location Ast Requirements HIR Compiler Hierarchy Utils gtest_main pthread ${llvm_libs})

add_dependencies(build_test clawn_test)

include(GoogleTest)