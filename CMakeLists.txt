cmake_minimum_required(VERSION 3.10)
project(Battleship)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar Boost
find_package(Boost REQUIRED COMPONENTS system)

# Agregar los archivos fuente
set(SOURCES
    main.cpp
    player.cpp
    game.cpp
    battleship_server.cpp
)

# Agregar los archivos de cabecera
set(HEADERS
    player.h
    game.h
    battleship_server.h
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crear el ejecutable
add_executable(servidor ${SOURCES} ${HEADERS})

# Vincular con Boost
target_link_libraries(servidor PRIVATE Boost::system)

# Configurar directorios de inclusión
target_include_directories(servidor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Configurar opciones de compilación
if(MSVC)
    target_compile_options(servidor PRIVATE /W4)
else()
    target_compile_options(servidor PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Configurar el directorio de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 