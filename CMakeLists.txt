cmake_minimum_required(VERSION 3.10)
project(Battleship)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar Boost
find_package(Boost REQUIRED COMPONENTS system)

# Agregar el ejecutable
add_executable(servidor servidor.cpp)

# Vincular con Boost
target_link_libraries(servidor PRIVATE Boost::system) 