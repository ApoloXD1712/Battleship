# Battleship

# Introducción
## Contexto y justificación
Este documento determina el desarrollo de un servidor del juego de batalla naval con el objetivo de ser capaz de soportar múltiples partidas simultáneas entre distintas parejas de jugadores. Este desarrollo tiene como objetivo el crear un espacio escalable y de calidad, en el que la comunicación se produzca de forma eficiente, utilizando para ello un protocolo de comunicación propio.
El objetivo general del proyecto es gestionar las partidas de forma eficiente y segura, manteniendo una buena calidad en la integridad de los datos y en la experiencia del usuario. Para ello, para la implementación del servidor se ha utilizado el lenguaje C++, junto a tecnologías de redes y despliegue en AWS.
El alcance del proyecto cubre la gestión de sesiones de juego, el control de las validaciones de los movimientos realizados en el juego, la baja latencia, así como también la comunicación entre los jugadores.
# Desarrollo
Cuando empezamos a desarrollar, separamos por clases los componentes principales que tendría el código, los cuales se dividen en:
* Servidor (Desarrollado en C++):
    - Battleship_server.h/cpp: Este se encarga de manejar las conexiones y la lógica del juego de batalla naval
    - Player.h/cpp: Define la clase Player que representa a cada jugador
    - Game.h/cpp: Maneja la lógica del juego de batalla naval (ubicación de los barcos, movimientos, terminación del juego, visualización del juego, etc)
    - Config.h: Archivo de configuración
    - Main.cpp: Punto de entrada del servidor

* Cliente (Desarrollado en Python):
    - Client.py: Cliente para conectarse al servidor
    - Juego.py: Lógica del juego en el lado del Cliente

Ahora, teniendo en cuenta lo anteriormente mencionado, la arquitectura general del juego se describe así:


