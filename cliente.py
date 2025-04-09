
import asyncio
import threading
import tkinter as tk
from tkinter import messagebox
import websockets

SERVER_IP = "34.238.123.42"
PORT = 8080
BOARD_SIZE = 5

class BattleshipClient:
    def __init__(self, root):
        self.root = root
        self.root.title("Battleship")
        self.buttons = [[None for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]
        self.nickname = None
        self.opponent_name = "Esperando oponente..."
        self.websocket = None
        self.last_move = None
        self.status_label = None
        self.turn_label = None
        self.board_enabled = False
        self.create_login_screen()

    def create_login_screen(self):
        self.clear_window()
        tk.Label(self.root, text="Ingresa tu nickname:").pack()
        self.nick_entry = tk.Entry(self.root)
        self.nick_entry.pack()
        tk.Button(self.root, text="Conectar", command=self.start_game).pack()

    def clear_window(self):
        for widget in self.root.winfo_children():
            widget.destroy()

    def create_board(self):
        self.clear_window()

        top_frame = tk.Frame(self.root)
        top_frame.pack()

        self.status_label = tk.Label(top_frame, text=f"Jugador: {self.nickname} | Rival: {self.opponent_name}", font=('Arial', 12, 'bold'))
        self.status_label.pack()

        self.turn_label = tk.Label(self.root, text="⏳ Esperando emparejamiento...", font=('Arial', 10))
        self.turn_label.pack()

        frame = tk.Frame(self.root)
        frame.pack(pady=10)

        for x in range(BOARD_SIZE):
            for y in range(BOARD_SIZE):
                btn = tk.Button(frame, text="~", width=4, height=2,
                                state='disabled',
                                command=lambda x=x, y=y: self.send_move(x, y))
                btn.grid(row=x, column=y)
                self.buttons[x][y] = btn

    def set_board_state(self, state):
        for row in self.buttons:
            for btn in row:
                btn.config(state=state)
        self.board_enabled = (state == 'normal')

    def start_game(self):
        self.nickname = self.nick_entry.get()
        if not self.nickname:
            messagebox.showerror("Error", "Debes ingresar un nickname.")
            return
        self.create_board()
        threading.Thread(target=self.run_client, daemon=True).start()

    async def connect(self):
        uri = f"ws://{SERVER_IP}:{PORT}"
        self.websocket = await websockets.connect(uri)
        await self.websocket.send(f"NICKNAME:{self.nickname}")
        await self.websocket.send("READY")

    async def handle_messages(self):
        while True:
            response = await self.websocket.recv()
            response = response.strip()

            if response == "YOU_WIN":
                self.mark_last_move("X", "green")
                self.turn_label.config(text="🎉 ¡Ganaste el juego!")
                messagebox.showinfo("¡Victoria!", "🎉 ¡Ganaste el juego!")
                self.set_board_state('disabled')
                break
            elif response == "YOU_LOSE":
                self.turn_label.config(text="☠️ Has perdido el juego.")
                messagebox.showinfo("Derrota", "☠️ Has perdido el juego.")
                self.set_board_state('disabled')
                break
            elif response == "NOT_YOUR_TURN":
                self.turn_label.config(text="⏳ No es tu turno.")
                self.set_board_state('disabled')
            elif response == "HIT":
                self.mark_last_move("X", "red")
                self.turn_label.config(text="🔥 ¡Impacto!")
            elif response == "SUNK":
                self.mark_last_move("X", "black")
                self.turn_label.config(text="🚢 ¡Barco hundido!")
                messagebox.showinfo("Barco Hundido", "🚢 ¡Hundiste un barco!")
            elif response == "MISS":
                self.mark_last_move("O", "light blue")
                self.turn_label.config(text="💨 Fallaste el tiro.")
            elif "MATCH_FOUND" in response:
                self.turn_label.config(text="🎯 ¡Partida encontrada! Esperando turno...")
            elif "YOUR_TURN" in response:
                self.turn_label.config(text="🎯 Es tu turno.")
                self.set_board_state('normal')
            elif "WAIT_TURN" in response:
                self.turn_label.config(text="🕒 Esperando turno del oponente.")
                self.set_board_state('disabled')
            elif response.startswith("OPPONENT|"):
                self.opponent_name = response.split("|")[1]
                self.status_label.config(text=f"Jugador: {self.nickname} | Rival: {self.opponent_name}")
            else:
                print("Respuesta desconocida:", response)

    def mark_last_move(self, text, color):
        if self.last_move:
            x, y = self.last_move
            self.buttons[x][y].config(text=text, bg=color)

    def coords_to_label(self, x, y):
        col_letter = chr(ord('a') + y)
        return f"{col_letter}{x}"

    async def send_move_async(self, x, y):
        if self.board_enabled:
            self.last_move = (x, y)
            coord = self.coords_to_label(x, y)
            await self.websocket.send(f"FIRE:{coord}")
            self.set_board_state('disabled')  # bloquear hasta nueva respuesta

    def send_move(self, x, y):
        asyncio.run_coroutine_threadsafe(self.send_move_async(x, y), self.loop)

    def run_client(self):
        self.loop = asyncio.new_event_loop()
        asyncio.set_event_loop(self.loop)
        self.loop.run_until_complete(self.connect())
        self.loop.create_task(self.handle_messages())
        self.loop.run_forever()

if __name__ == "__main__":
    root = tk.Tk()
    app = BattleshipClient(root)
    root.mainloop()
