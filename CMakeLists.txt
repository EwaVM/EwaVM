cmake_minimum_required(VERSION 3.12)

project(ewasvm)

aux_source_directory(ewasvm source_src)
aux_source_directory(ewasvm/core core_src)
aux_source_directory(ewasvm/opgen opgen_src)

add_compile_options(-fvisibility=hidden)
add_compile_options(-fpermissive)

include_directories(${CMAKE_SOURCE_DIR}/ewasvm/include ${CMAKE_SOURCE_DIR}/sljit/sljit_src)

add_library(ewasvm STATIC ${source_src} ${core_src} ${opgen_src} ewasvm/sljit/sljit_src/sljitLir.c)

# add_compile_options(-DEXPORT_SHARED=1)

add_library(ewasvm_shared SHARED ${source_src} ${core_src} ${opgen_src} ewasvm/sljit/sljit_src/sljitLir.c)
target_include_directories(ewasvm PUBLIC ewasvm/include)