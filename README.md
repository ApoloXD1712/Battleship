# Battleship

# Introducción
En la actualidad, la implementación de sistemas interactivos que permitan la comunicación en tiempo real entre usuarios es cada vez más común, especialmente en aplicaciones orientadas al entretenimiento digital. Por esta razón, este proyecto se basa en el desarrollo de una versión del clásico juego Batalla Naval utilizando un modelo de comunicación cliente/servidor.

La aplicación permite que dos jugadores, conectados de forma remota, se enfrenten en una partida uno a uno, gestionando de manera eficiente el intercambio de mensajes y movimientos a través de una conexión de red. Todo el flujo de comunicación está diseñado para funcionar de forma concurrente, asegurando una experiencia fluida y sincronizada entre ambos participantes.

El sistema está desarrollado en C++ y hace uso de sockets para establecer la comunicación entre el cliente y el servidor. Además, el protocolo que define el formato y la interpretación de los mensajes entre jugadores ha sido implementado de forma personalizada, adaptándose a las necesidades específicas del juego.
## Contexto y justificación
El objetivo general del proyecto es gestionar las partidas de forma eficiente y segura, manteniendo una buena calidad en la integridad de los datos y en la experiencia del usuario. Para ello, para la implementación del servidor se ha utilizado el lenguaje C++, junto a tecnologías de redes y despliegue en AWS.
El alcance del proyecto cubre la gestión de sesiones de juego, el control de las validaciones de los movimientos realizados en el juego, la baja latencia, así como también la comunicación entre los jugadores.
# Desarrollo
Cuando empezamos a desarrollar, separamos por clases los componentes principales que tendría el código, los cuales se dividen en:
* Servidor (Desarrollado en C++):
    - Battleship_server.h/cpp: Este se encarga de manejar las conexiones entrantes, gestionando la lista de jugadores y coordinando los juegos activos, además de implementar el sistema de emparejamiento.
    - Player.h/cpp: Define la clase Player que representa a cada jugador conectado, manteniendo el estado del jugador y manejando la comunicación con el cliente.
    - Game.h/cpp: Maneja la lógica del juego de batalla naval, manejando así el tablero y los movimientos, controla también el flujo del juego.
    - Config.h: Archivo de configuración
    - Main.cpp: Punto de entrada del servidor

* Cliente (Desarrollado en Python):
    - Client.py: Cliente para conectarse al servidor
    - Juego.py: Lógica del juego en el lado del Cliente

Ahora, teniendo en cuenta lo anteriormente mencionado, la arquitectura general del juego se describe así:

El servidor usa Boost.Asio para manejar las conexiones de red. A la hora de manejar clientes se implementa entonces un sistema de múltiples hilos para manejar varios clientes.

## Características principales (A nivel del servidor)
* Manejo de conexiones TCP
* Sistema de logging
* Gestión de múltiples juegos simultáneamente hablando
* Control de turnos y tiempo límite

El cliente hace uso de websockets y cuenta con una interfaz de línea de comandos simple
## Características principales (A nivel del cliente)
* Conexión al servidor
* Envío de movimientos por parte de ambos jugadores
* Sistema de logging local
* Manejo de nicknames (Para jugadores)

## Flujo del Juego
Este flujo será descrito por pasos, los pasos son:
1. Los clientes se conectan al servidor
2. Los jugadores ingresan sus nicknames
3. El servidor se encarga de emparejar a los jugadores
4. Los jugadores realizan movimientos de manera alternada
5. El juego continúa hasta que un jugador gana o se desconecta

## Tecnologías utilizadas
* C++ con Boost.Asio para el servidor
* Python con websockets para el cliente
* CMake para la compilación del servidor
* Sistema de logging para debugging



