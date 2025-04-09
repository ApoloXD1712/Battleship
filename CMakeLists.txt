cmake_minimum_required(VERSION 3.10)
project(Battleship)

# Usar C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Boost
find_package(Boost REQUIRED COMPONENTS system)

# Archivos fuente
set(SOURCES
    main.cpp               # o servidor.cpp si mantenés ese nombre
    player.cpp
    game.cpp
    battleship_server.cpp
    websocket_session.cpp
)

# Archivos de encabezado (opcional, para IDEs)
set(HEADERS
    player.h
    game.h
    battleship_server.h
)

# Crear el ejecutable
add_executable(servidor ${SOURCES} ${HEADERS})

# Incluir directorios
target_include_directories(servidor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enlazar Boost
target_link_libraries(servidor PRIVATE Boost::system pthread)

# Opciones de compilación
if(MSVC)
    target_compile_options(servidor PRIVATE /W4)
else()
    target_compile_options(servidor PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Directorio de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)