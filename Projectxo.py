import tkinter as tk
from tkinter import messagebox

root = tk.Tk()
root.title("เกม XO สองผู้เล่น")
root.geometry("327x550")
root.configure(bg="#F7F7F7")
root.resizable(False,False)

board = [""] * 9
current_player = "X" 
scores = {"X": 0, "O": 0, "Draw": 0}
game_over = False 

WINS = [(0, 1, 2), (3, 4, 5), (6, 7, 8), 
        (0, 3, 6), (1, 4, 7), (2, 5, 8), 
        (0, 4, 8), (2, 4, 6)] 

def check_winner(b = None): 
    try:
        if b is None: 
            state = board 
        else: 
            state = b 
        
        for a, b, c in WINS: 
            if state[a] == state[b] == state[c] != "": 
                return state[a] 

        if "" not in state: 
            return "Draw" 
        else: 
            return None 
    except Exception as e:
            messagebox.showerror("ข้อผิดพลาด", "เกิดข้อผิดพลาดในการตรวจสอบผู้ชนะ:")
            return None

def on_click(i): 
    try:
        global current_player, game_over 

        if buttons[i] ["text"] or game_over: 
            return 

        buttons[i] ["text"] = current_player 

        if current_player == "X": 
            buttons[i]["bg"] = "#FF5454" 
        else: 
            buttons[i]["bg"] = "#72CDF1"  

        board[i] = current_player 
        winner = check_winner() 

        if winner: 
            game_over = True 
            scores[winner] += 1 

            if winner == "Draw": 
                msg = "เกมเสมอ!" 
            else: 
                msg = f"ผู้เล่น {winner} ชนะ!" 

            messagebox.showinfo("ผลเกม", msg)
            status_label.config(text=msg) 
            update_score_display()

        else:
            if current_player == "X": 
                
                current_player = "O"
            else:
                current_player = "X" 
            status_label.config(text=f"🎯ถึงตา: {current_player}") 
    except Exception as e:
            messagebox.showerror("ข้อผิดพลาด", "เกิดข้อผิดพลาดในการเล่น: ")

def reset_game(): 
    try:
        global current_player, board, game_over  
        current_player, board, game_over = "X", [""] * 9, False 
        for btn in buttons: 
            btn.config(text="", bg="#C79B99", state="normal")  
        status_label.config(text="ถึงตา: X") 
    except Exception as e:
        messagebox.showerror("ข้อผิดพลาด","เกิดข้อผิดพลาดในการอัปเดตคะแนน:")

def update_score_display(): 
    try:
        score_label.config( text=
                           f"คะแนน  X: {scores['X']}  |  O: {scores['O']}  |  เสมอ: {scores['Draw']}")
    except Exception as e:
        messagebox.showerror("ข้อผิดพลาด","เกิดข้อผิดพลาดในการรีเซ็ตคะแนน:")



mode_label = tk.Label(root, text="🎮 เกม XO", font=("Arial", 20,"bold"),  
                      bg="#F7F7F7", fg="#3C3C3C")
mode_label.grid(row=0, column=0, columnspan=3, pady=15)

status_label = tk.Label(root, text="🎯 ถึงตา: X", font=("Arial", 14, "bold"), 
                        bg="#F7F7F7", fg="#3C3C3C")
status_label.grid(row=5, column=0, columnspan=3)

score_label = tk.Label(root,text="คะแนน  X: 0  |  O: 0  |  เสมอ: 0", font=("Arial", 12),  
                       bg="#F7F7F7", fg="#007B83")  
score_label.grid(row=6, column=0, columnspan=3, pady=5)

buttons = []
for i in range(9):
    btn = tk.Button(root,  text="",  font=("Arial", 24, "bold"),  width=5,  height=2,  
                    command=lambda i=i: on_click(i),  bg="#C79B99")  
    btn.grid(row=i // 3 + 1, column=i % 3, padx=2, pady=2)
    buttons.append(btn)

reset_button = tk.Button(root,  text="🔄 รีเซ็ตเกม",  font=("Arial", 12),  
                         command=reset_game, bg="#87CEEB")  
reset_button.grid(row=4, column=0, columnspan=3, pady=10)

reset_score_btn = tk.Button(root,text="🧹 รีเซ็ตคะแนน",font=("Arial", 12),command=lambda:
         (scores.update({"X": 0, "O": 0, "Draw": 0}), update_score_display()),bg="#FFA500", width=20)  
reset_score_btn.grid(row=7, column=0, columnspan=3, pady=10)



root.mainloop()