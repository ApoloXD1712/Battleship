# Battleship - Proyecto de Telemática

## Desarrolladores

- Alejandro Cadavid Osorio
- Juan Alberto Rodríguez

## Introducción

En este proyecto se implementa una versión del clásico juego **Batalla Naval**, desarrollado como parte del curso de Telemática, con el objetivo de aplicar conceptos clave de programación en red y diseño de protocolos de comunicación. Utilizando sockets TCP, se diseñó una arquitectura cliente/servidor donde dos jugadores pueden competir remotamente, enviando y recibiendo mensajes mediante un protocolo personalizado.

La comunicación se basa en la API Berkeley de sockets, y tanto el manejo de turnos como el estado del juego se centralizan en el servidor. Este escenario permite poner en práctica conceptos como concurrencia, sincronización, validación de mensajes y manejo de errores.

## Desarrollo

### Arquitectura General

El sistema se compone de tres elementos principales:

- **Cliente**: Interfaz elaborada en Tkinter (Herramienta de Python) que permite al jugador interactuar con el juego dándole una mejor experiencia.
- **Servidor**: Componente en C++ que gestiona el estado del juego, aplica las reglas, y administra las conexiones.
- **Protocolo personalizado**: Define el formato y reglas de intercambio de mensajes entre cliente y servidor.

### Detalles Técnicos

- **Lenguaje del servidor**: C++ (uso obligatorio).
- **Lenguaje del cliente**: Python 3.x.
- **Protocolo implementado**: TCP transformado en WebSocket (mediante el cliente)
- **Concurrencia**: Implementada mediante hilos para permitir múltiples partidas simultáneas.
- **Librerías utilizadas**:
  * Boost.Asio / Boost.Beast: Creación del servidor WebSocket
  * Librería estándar de C++
  * CMake: Construcción del proyecto
  * Websockets: Conexión al servidor en tiempo real
  * Asyncio: Manejo asíncrono de las corrutinas (envío y recepción de mensajes)
  * Tkinter: Interfaz de usuario, botones y ventanas

### Ejecución del servidor (Si se tiene el código completo)

```bash
git clone https://github.com/ApoloXD1712/Battleship.git
```

```bash
./servidor
```

### Ejecución del cliente (Para otros jugadores)

```bash
./cliente.py
```

## Protocolo de Comunicación

El protocolo implementado sigue el formato de texto plano con mensajes estructurados del tipo:

```
<TIPO_MENSAJE>:<DATOS>
```

### Ejemplo de Vocabulario de Mensajes

| Tipo de Mensaje | Datos | Descripción |
|------------------|-------|-------------|
| REGISTER         | nickname,email | Registro del jugador |
| PLACE_SHIPS      | posiciones      | Envío de las posiciones de los barcos |
| FIRE             | coordenada      | Disparo a coordenada enemiga |
| RESULT           | hit/miss/sunk   | Resultado del disparo |
| GAME_OVER        | win/lose        | Finalización de partida |
| DISCONNECT       | mensaje         | Desconexión del cliente |

Este protocolo es responsable de mantener sincronizados los estados de ambos jugadores y facilitar el desarrollo fluido del juego.

## Aspectos Logrados

- Comunicación bidireccional estable entre cliente y servidor.
- Registro de logs tanto en cliente como en servidor.
- Manejo de múltiples partidas concurrentes en el servidor.
- Validación de reglas del juego (turnos, aciertos, hundimientos).
- Visualización clara en consola (tableros propios y del enemigo).
- Emparejamiento automático de jugadores.
- Detección de desconexiones.
- Implementación de una interfaz gráfica sencilla

## Posibles Mejoras

- Persistencia de estadísticas de partidas.
- Mejor manejo de errores inesperados.
- Adición de comandos para chat entre jugadores.

## Conclusiones

Este proyecto ha sido fundamental para aplicar de forma práctica los conceptos teóricos vistos durante el curso, como el uso de sockets, programación concurrente, diseño de protocolos y sincronización entre múltiples clientes. La implementación de un juego multijugador en red nos permitió comprender de manera más profunda cómo funcionan las comunicaciones en tiempo real y los desafíos que conlleva la coordinación de múltiples actores distribuidos.

## Referencias

- [Guía de Sockets Beej](https://beej.us/guide/bgnet/)
- [TCP Server-Client en C++](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)
- [Sockets en Python](https://docs.python.org/3/library/socket.html)
- [CMake](https://cmake.org/)



