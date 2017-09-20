cmake_minimum_required(VERSION 3.8)
project(Tarea_SO)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        funcionesAlf.c
        funcionesAlf.h
        header.h
        init.c
        init.h
        main2.c)

add_executable(Tarea_SO ${SOURCE_FILES})