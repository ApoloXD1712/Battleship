import asyncio
import websockets
import logging

SERVER_IP = "your-aws-instance-ip"
PORT = 8080
LOG_FILE = "client_log.txt"

logging.basicConfig(filename=LOG_FILE, level=logging.INFO,
                    format='%(asctime)s - %(message)s')

async def send_message(websocket, message):
    await websocket.send(message)
    logging.info(f"Sent: {message}")
    response = await websocket.recv()
    logging.info(f"Received: {response}")
    return response

async def play_battleship():
    uri = f"ws://{SERVER_IP}:{PORT}"
    async with websockets.connect(uri) as websocket:
        nickname = input("Ingresa tu nickname: ")
        await send_message(websocket, f"NICKNAME|{nickname}")
        
        while True:
            command = input("Introduce tu movimiento (Ej: A3) o escribe 'exit' para salir: ")
            if command.lower() == 'exit':
                await send_message(websocket, "EXIT")
                break
            response = await send_message(websocket, f"MOVE|{command}")
            print(f"Respuesta del servidor: {response}")

asyncio.run(play_battleship())
