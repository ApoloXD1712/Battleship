import random

class Battleship:
    def __init__(self, size=5):
        self.size = size
        self.board = [['~' for _ in range(size)] for _ in range(size)]
        self.ships = []
        self.place_ships()
    
    def place_ships(self, num_ships=3):
        for _ in range(num_ships):
            while True:
                x, y = random.randint(0, self.size - 1), random.randint(0, self.size - 1)
                if (x, y) not in self.ships:
                    self.ships.append((x, y))
                    break
    
    def display_board(self):
        print("  " + " ".join(str(i) for i in range(self.size)))
        for idx, row in enumerate(self.board):
            print(str(idx) + " " + " ".join(row))
    
    def check_move(self, x, y):
        if (x, y) in self.ships:
            self.board[x][y] = 'X'
            self.ships.remove((x, y))
            return "HIT"
        else:
            self.board[x][y] = 'O'
            return "MISS"
    
    def is_game_over(self):
        return len(self.ships) == 0

def play_game():
    game = Battleship()
    print("Bienvenido a Battleship!")
    game.display_board()
    
    while not game.is_game_over():
        try:
            x, y = map(int, input("Introduce coordenadas (x y): ").split())
            if x < 0 or x >= game.size or y < 0 or y >= game.size:
                print("Coordenadas fuera del tablero. Intenta de nuevo.")
                continue
            result = game.check_move(x, y)
            print(result)
            game.display_board()
        except ValueError:
            print("Entrada inválida. Usa formato: x y")
    
    print("¡Ganaste! Has hundido todos los barcos.")
    print("Fin del juego, gracias por jugar!")

if __name__ == "__main__":
    play_game()