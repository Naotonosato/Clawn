cmake_minimum_required(VERSION 2.8)

add_custom_target(build_clawn)

set(CMAKE_CXX_COMPILER "g++" CACHE STRING "g++ compiler" FORCE)

set(LIB_MAJOR_VERSION "0")
set(LIB_MINOR_VERSION "1")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")

project(Clawn)

find_package(BISON REQUIRED)

find_package(FLEX REQUIRED)

find_package(LLVM 9 REQUIRED)
set(FLEX_INCLUDE_DIRS /usr/local/opt/flex/include)
set(FLEX_LIBRARIES /usr/local/opt/flex/lib)
set(BISON_INCLUDE_DIRS usr/local/opt/bison/include)

bison_target(parser ${CMAKE_SOURCE_DIR}/src/lib/compiler/parser/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS --debug)
flex_target(prelexer ${CMAKE_SOURCE_DIR}/src/lib/compiler/parser/prelexer.ll ${CMAKE_CURRENT_BINARY_DIR}/prelexer.cpp COMPILE_FLAGS "--debug -Ppre")
flex_target(lexer ${CMAKE_SOURCE_DIR}/src/lib/compiler/parser/lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp COMPILE_FLAGS --debug)

ADD_FLEX_BISON_DEPENDENCY(lexer parser)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${LLVM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${LLVM_INCLUDE_DIRS} ${FLEX_INCLUDE_DIRS})
set(CLAWN_LIB_DIR "${CMAKE_SOURCE_DIR}/src/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
execute_process(COMMAND llvm-config --libdir OUTPUT_VARIABLE LLVM_LIBS_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --cxxflags --ldflags  --system-libs --libs all OUTPUT_VARIABLE LLVM_OPTIONS)
string(REPLACE "\n" " " LLVM_OPTIONS ${LLVM_OPTIONS})
string(REPLACE "-fno-exceptions" " " LLVM_OPTIONS ${LLVM_OPTIONS})
string(REPLACE "-std=c++14" " " LLVM_OPTIONS ${LLVM_OPTIONS})
message(${LLVM_OPTIONS})
set(CMAKE_CXX_FLAGS "-std=c++17 -Wno-unused-command-line-argument  -Wno-unused-lambda-capture  -Wno-unused-private-field --debug -O2  ${LLVM_OPTIONS}")

set(ld_flags "--debug -O2 -Wno-unused-lambda-capture  -Wno-unused-private-field -Wno-unused-command-line-argument ${LLVM_LIBS_DIR}/liblldCOFF.a  ${LLVM_LIBS_DIR}/liblldCommon.a  ${LLVM_LIBS_DIR}/liblldCore.a  ${LLVM_LIBS_DIR}/liblldDriver.a  ${LLVM_LIBS_DIR}/liblldELF.a  ${LLVM_LIBS_DIR}/liblldMachO.a  ${LLVM_LIBS_DIR}/liblldMinGW.a  ${LLVM_LIBS_DIR}/liblldReaderWriter.a  ${LLVM_LIBS_DIR}/liblldWasm.a  ${LLVM_LIBS_DIR}/liblldYAML.a ${LLVM_OPTIONS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}")



llvm_map_components_to_libnames(llvm_libs support core irreader) 

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}")
file(GLOB CPP_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

#add_subdirectory(${PROJECT_SOURCE_DIR}/test)
 

add_subdirectory(${CLAWN_LIB_DIR}/location)
add_subdirectory(${CLAWN_LIB_DIR}/ast)
add_subdirectory(${CLAWN_LIB_DIR}/compiler)
add_subdirectory(${CLAWN_LIB_DIR}/requirement)
add_subdirectory(${CLAWN_LIB_DIR}/hir)
add_subdirectory(${CLAWN_LIB_DIR}/mir)
add_subdirectory(${CLAWN_LIB_DIR}/hierarchy)
add_subdirectory(${CLAWN_LIB_DIR}/utils)
add_subdirectory(${CLAWN_LIB_DIR}/resource)

add_library(Parser STATIC
	${BISON_parser_OUTPUTS}
	${FLEX_prelexer_OUTPUTS}
	${FLEX_lexer_OUTPUTS}
	)

add_executable(clawn ${CLAWN_LIB_DIR}/main.cpp)
target_link_libraries(clawn Parser Location Ast HIR MIR Compiler Hierarchy Utils Requirements Resource ${llvm_libs})

add_dependencies(build_clawn clawn)

add_custom_target(bison_patch ALL COMMAND "python3" "../src/lib/compiler/parser/patch.py")
#add_custom_target(format ALL python3 ../dev_utils/format_cpp.py ../src)
add_custom_target(copy_builtins_cpp ALL "cp" "../src/lib/compiler/builtins.cpp" "builtins.cpp")
add_custom_target(copy_builtins_hpp ALL "cp" "../src/lib/compiler/builtins.hpp" "builtins.hpp")
add_custom_target(copy_tools ALL "cp" "../tools/cridge.py" "cridge.py")
