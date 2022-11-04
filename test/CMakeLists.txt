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


set(ld_flags "--debug -O0 -Wno-unused-command-line-argument /usr/local/opt/llvm/lib/liblldCOFF.a /usr/local/opt/llvm/lib/liblldCommon.a /usr/local/opt/llvm/lib/liblldCore.a /usr/local/opt/llvm/lib/liblldDriver.a /usr/local/opt/llvm/lib/liblldELF.a /usr/local/opt/llvm/lib/liblldMachO.a /usr/local/opt/llvm/lib/liblldMinGW.a /usr/local/opt/llvm/lib/liblldReaderWriter.a /usr/local/opt/llvm/lib/liblldWasm.a /usr/local/opt/llvm/lib/liblldYAML.a -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ld_flags}")
set(CMAKE_CXX_FLAGS "-std=c++17 --debug -O0 -Wno-unused-command-line-argument -I/usr/local/Cellar/llvm/9.0.0_1/include -std=c++17 -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -L/usr/local/Cellar/llvm/9.0.0_1/lib -Wl,-search_paths_first -Wl,-headerpad_max_install_names -lLLVMXRay -lLLVMWindowsManifest -lLLVMTextAPI -lLLVMTableGen -lLLVMSymbolize -lLLVMDebugInfoPDB -lLLVMOrcJIT -lLLVMJITLink -lLLVMObjectYAML -lLLVMMCA -lLLVMLTO -lLLVMPasses -lLLVMObjCARCOpts -lLLVMLineEditor -lLLVMLibDriver -lLLVMInterpreter -lLLVMFuzzMutate -lLLVMMCJIT -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMDlltoolDriver -lLLVMOption -lLLVMDebugInfoGSYM -lLLVMCoverage -lLLVMCoroutines -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMX86Utils -lLLVMX86Info -lLLVMWebAssemblyDisassembler -lLLVMWebAssemblyCodeGen -lLLVMWebAssemblyDesc -lLLVMWebAssemblyAsmParser -lLLVMWebAssemblyInfo -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc -lLLVMSparcInfo -lLLVMRISCVDisassembler -lLLVMRISCVCodeGen -lLLVMRISCVAsmParser -lLLVMRISCVDesc -lLLVMRISCVUtils -lLLVMRISCVInfo -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMMSP430Disassembler -lLLVMMSP430CodeGen -lLLVMMSP430AsmParser -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc -lLLVMMipsInfo -lLLVMLanaiDisassembler -lLLVMLanaiCodeGen -lLLVMLanaiAsmParser -lLLVMLanaiDesc -lLLVMLanaiInfo -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen -lLLVMHexagonAsmParser -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMBPFDisassembler -lLLVMBPFCodeGen -lLLVMBPFAsmParser -lLLVMBPFDesc -lLLVMBPFInfo -lLLVMARMDisassembler -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMUtils -lLLVMARMInfo -lLLVMAMDGPUDisassembler -lLLVMAMDGPUCodeGen -lLLVMMIRParser -lLLVMipo -lLLVMInstrumentation -lLLVMVectorize -lLLVMLinker -lLLVMIRReader -lLLVMAsmParser -lLLVMAMDGPUAsmParser -lLLVMAMDGPUDesc -lLLVMAMDGPUUtils -lLLVMAMDGPUInfo -lLLVMAArch64Disassembler -lLLVMMCDisassembler -lLLVMAArch64CodeGen -lLLVMGlobalISel -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMDebugInfoDWARF -lLLVMCodeGen -lLLVMTarget -lLLVMScalarOpts -lLLVMInstCombine -lLLVMAggressiveInstCombine -lLLVMTransformUtils -lLLVMBitWriter -lLLVMAnalysis -lLLVMProfileData -lLLVMObject -lLLVMBitReader -lLLVMBitstreamReader -lLLVMCore -lLLVMRemarks -lLLVMAArch64AsmParser -lLLVMMCParser -lLLVMAArch64Desc -lLLVMMC -lLLVMDebugInfoCodeView -lLLVMDebugInfoMSF -lLLVMBinaryFormat -lLLVMAArch64Utils -lLLVMAArch64Info -lLLVMSupport -lLLVMDemangle -O0")

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