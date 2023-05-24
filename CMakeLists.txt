cmake_minimum_required(VERSION 3.12)

project(EwaVM)

aux_source_directory(EwaVM source_src)
aux_source_directory(EwaVM/core core_src)
aux_source_directory(EwaVM/opgen opgen_src)

add_compile_options(-ffunction-sections -fdata-sections -fvisibility=hidden -fvisibility-inlines-hidden -fpermissive)

include_directories(${CMAKE_SOURCE_DIR}/EwaVM/include ${CMAKE_SOURCE_DIR}/sljit/sljit_src)

add_library(ewavm_shared SHARED ${source_src} ${core_src} ${opgen_src} EwaVM/sljit/sljit_src/sljitLir.c)
add_library(ewavm STATIC ${source_src} ${core_src} ${opgen_src} EwaVM/sljit/sljit_src/sljitLir.c)
target_include_directories(ewavm PUBLIC EwaVM/include)