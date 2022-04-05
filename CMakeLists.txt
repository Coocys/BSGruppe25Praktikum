cmake_minimum_required(VERSION 3.10.2)
project(BSGruppe25Praktikum C)

set(CMAKE_C_STANDARD 99)

add_executable(BSGruppe25Praktikum main.c keyValStore.c keyValStore.h server.h server.c)
