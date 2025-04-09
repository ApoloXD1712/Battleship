import asyncio
import websockets
import logging
import os

SERVER_IP = "52.207.215.7"
PORT = 8080
LOG_FILE = "client_log.txt"

# Inicializar el tablero del jugador (solo para mostrar sus disparos)
BOARD_SIZE = 5
board = [['~' for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]

logging.basicConfig(filename=LOG_FILE, level=logging.INFO,
                    format='%(asctime)s - %(message)s')


def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')


def display_board():
    print("   " + " ".join(str(i) for i in range(BOARD_SIZE)))
    for idx, row in enumerate(board):
        print(f"{idx}  " + " ".join(row))
    print()


def update_board(x, y, result):
    if result == "HIT" or result == "SUNK":
        board[x][y] = 'X'
    elif result == "MISS":
        board[x][y] = 'O'


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
            clear_screen()
            display_board()
            command = input("Introduce tu movimiento (Ej: 2 3) o escribe 'exit' para salir: ")

            if command.lower() == 'exit':
                await send_message(websocket, "EXIT")
                break

            try:
                x_str, y_str = command.strip().split()
                x, y = int(x_str), int(y_str)
            except ValueError:
                print("Formato inválido. Usa: fila columna (Ej: 1 2)")
                continue

            response = await send_message(websocket, f"MOVE|{x},{y}")
            response = response.strip()

            if response == "NOT_YOUR_TURN":
                print("⏳ No es tu turno. Espera al oponente.")
                await asyncio.sleep(2)
                continue

            if response == "YOU_WIN":
                update_board(x, y, "HIT")
                clear_screen()
                display_board()
                print("🎉 ¡Ganaste el juego!")
                break
            elif response == "YOU_LOSE":
                clear_screen()
                display_board()
                print("☠️  Has perdido el juego.")
                break
            elif response == "HIT":
                print("🔥 Impacto directo.")
                update_board(x, y, "HIT")
            elif response == "SUNK":
                print("🚢 ¡Hundiste un barco!")
                update_board(x, y, "SUNK")
            elif response == "MISS":
                print("💨 Agua...")
                update_board(x, y, "MISS")
            else:
                print(f"Respuesta desconocida del servidor: {response}")

            await asyncio.sleep(2)


if __name__ == "__main__":
    asyncio.run(play_battleship())
