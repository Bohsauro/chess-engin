import tkinter as tk
from tkinter import messagebox
import subprocess
import threading
import os

class ChessGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Sauro Chess Engine")

        self.board_size = 400
        self.square_size = self.board_size // 8
        self.canvas = tk.Canvas(self.root, width=self.board_size + 200, height=self.board_size)
        self.canvas.pack()

        self.canvas.bind("<Button-1>", self.on_click)

        # Ensure the engine is compiled
        if not os.path.exists("./chess_engin"):
             messagebox.showerror("Error", "chess_engin binary not found. Please compile it first.")

        self.engine_process = subprocess.Popen(
            ["./chess_engin"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )

        self.history = []
        self.selected_square = None
        self.turn = "white"
        self.engine_thinking = False

        self.canvas.create_rectangle(self.board_size, 0, self.board_size + 200, self.board_size, fill="#f0f0f0")
        self.stats_text = self.canvas.create_text(self.board_size + 100, 100, text="Stats:\nNodes: 0\nScore: 0", font=("Arial", 12))
        self.thinking_label = self.canvas.create_text(self.board_size + 100, 200, text="", fill="red", font=("Arial", 12, "bold"))

        self.board = self.get_initial_board()
        self.draw_board()

    def get_initial_board(self):
        b = [[None for _ in range(8)] for _ in range(8)]
        for i in range(8):
            b[1][i] = 'P'
            b[6][i] = 'p'
        pieces = ['R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R']
        for i, p in enumerate(pieces):
            b[0][i] = p
            b[7][i] = p.lower()
        return b

    def draw_board(self):
        self.canvas.delete("piece")
        self.canvas.delete("square")
        for r in range(8):
            for c in range(8):
                x1, y1 = c * self.square_size, (7 - r) * self.square_size
                x2, y2 = x1 + self.square_size, y1 + self.square_size
                color = "#eeeed2" if (r + c) % 2 == 1 else "#769656"
                if self.selected_square == (r, c):
                    color = "#f7f769"
                self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, tags="square", outline="")

                piece = self.board[r][c]
                if piece:
                    unicode_pieces = {
                        'P': '♙', 'N': '♘', 'B': '♗', 'R': '♖', 'Q': '♕', 'K': '♔',
                        'p': '♟', 'n': '♞', 'b': '♝', 'r': '♜', 'q': '♛', 'k': '♚'
                    }
                    self.canvas.create_text(x1 + self.square_size//2, y1 + self.square_size//2,
                                            text=unicode_pieces[piece], font=("Arial", 30), tags="piece")

    def on_click(self, event):
        if self.engine_thinking or self.turn == "black":
            return

        c = event.x // self.square_size
        r = 7 - (event.y // self.square_size)

        if c >= 8 or r >= 8:
            return

        if self.selected_square:
            fr, fc = self.selected_square
            move_str = f"{chr(ord('a')+fc)}{fr+1}{chr(ord('a')+c)}{r+1}"

            if (fr, fc) != (r, c):
                piece = self.board[fr][fc]
                if piece == 'P' and r == 7:
                    move_str += 'q' # Auto-promote to queen
                self.apply_move(move_str)
                self.selected_square = None
                self.draw_board()
                if self.turn == "black":
                    self.engine_move()
            else:
                self.selected_square = None
                self.draw_board()
        else:
            if self.board[r][c] and self.board[r][c].isupper():
                self.selected_square = (r, c)
                self.draw_board()

    def apply_move(self, move_str):
        fc = ord(move_str[0]) - ord('a')
        fr = int(move_str[1]) - 1
        tc = ord(move_str[2]) - ord('a')
        tr = int(move_str[3]) - 1

        piece = self.board[fr][fc]
        target = self.board[tr][tc]

        # Castling
        if piece.upper() == 'K' and abs(fc - tc) == 2:
            if tc == 6: # Kingside
                self.board[tr][5] = self.board[tr][7]
                self.board[tr][7] = None
            elif tc == 2: # Queenside
                self.board[tr][3] = self.board[tr][0]
                self.board[tr][0] = None

        # En Passant
        if piece.upper() == 'P' and fc != tc and target is None:
            self.board[fr][tc] = None

        self.board[tr][tc] = piece
        self.board[fr][fc] = None

        # Promotion
        if len(move_str) > 4:
            promo = move_str[4]
            if piece.isupper(): self.board[tr][tc] = promo.upper()
            else: self.board[tr][tc] = promo.lower()

        self.history.append(move_str)
        self.turn = "black" if self.turn == "white" else "white"

    def engine_move(self):
        self.engine_thinking = True
        self.canvas.itemconfig(self.thinking_label, text="Sauro is thinking...")
        threading.Thread(target=self._query_engine).start()

    def _query_engine(self):
        cmd = f"position startpos moves {' '.join(self.history)}\ngo\n"
        self.engine_process.stdin.write(cmd)
        self.engine_process.stdin.flush()

        best_move = None
        nodes = 0
        score = 0
        while True:
            line = self.engine_process.stdout.readline()
            if not line: break
            if line.startswith("info"):
                parts = line.split()
                if "nodes" in parts:
                    nodes = parts[parts.index("nodes")+1]
                if "cp" in parts:
                    score = parts[parts.index("cp")+1]
            if line.startswith("bestmove"):
                parts = line.split()
                if len(parts) > 1:
                    best_move = parts[1]
                break

        self.root.after(0, lambda: self.finish_engine_move(best_move, nodes, score))

    def finish_engine_move(self, best_move, nodes, score):
        if best_move and best_move != "(none)":
            self.apply_move(best_move)
        else:
            messagebox.showinfo("Game Over", "Checkmate or Stalemate!")

        self.canvas.itemconfig(self.stats_text, text=f"Stats:\nNodes: {nodes}\nScore: {score}")
        self.canvas.itemconfig(self.thinking_label, text="")
        self.engine_thinking = False
        self.draw_board()

if __name__ == "__main__":
    root = tk.Tk()
    gui = ChessGUI(root)
    root.mainloop()
