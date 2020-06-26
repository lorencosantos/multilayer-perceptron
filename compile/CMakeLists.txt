cmake_minimum_required(VERSION 3.15)
project(main C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS -m32)
include_directories(include)
add_executable(main src/main.c
        src/back_propagation.c include/back_propagation.h
        src/classificador.c include/classificador.h
        src/contar_csv.c include/contar_csv.h
        src/escrever_csv.c include/escrever_csv.h
        src/forward_propagation.c include/forward_propagation.h
        src/ler_csv.c include/ler_csv.h
        src/parametros_io.c include/parametros_io.h
        src/treinador.c include/treinador.h
        )