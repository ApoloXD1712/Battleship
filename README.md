# Battleship

# Introducción
## Contexto y justificación
Este documento determina el desarrollo de un servidor del juego de batalla naval con el objetivo de ser capaz de soportar múltiples partidas simultáneas entre distintas parejas de jugadores. Este desarrollo tiene como objetivo el crear un espacio escalable y de calidad, en el que la comunicación se produzca de forma eficiente, utilizando para ello un protocolo de comunicación propio.
El objetivo general del proyecto es gestionar las partidas de forma eficiente y segura, manteniendo una buena calidad en la integridad de los datos y en la experiencia del usuario. Para ello, para la implementación del servidor se ha utilizado el lenguaje C++, junto a tecnologías de redes y despliegue en AWS.
El alcance del proyecto cubre la gestión de sesiones de juego, el control de las validaciones de los movimientos realizados en el juego, la baja latencia, así como también la comunicación entre los jugadores. 
