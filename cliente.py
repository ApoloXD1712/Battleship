import asyncio
import websockets

SERVER_IP = "your-aws-instance-ip"  # Reemplázalo con la IP de tu instancia EC2
PORT = 8080

async def play_battleship():
    uri = f"ws://{SERVER_IP}:{PORT}"
    async with websockets.connect(uri) as websocket:
        while True:
            move = input("Introduce tu movimiento (Ej: A3): ")
            await websocket.send(move)
            response = await websocket.recv()
            print(f"Respuesta del servidor: {response}")

asyncio.run(play_battleship())
