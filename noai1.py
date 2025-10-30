import tkinter as tk
from tkinter import messagebox

root = tk.Tk()
root.title("เกม XO สองผู้เล่น")
root.geometry("328x550")
root.resizable(False,False)

board = [""] * 9
current_player = "X" # สร้างลิสต์เก็บสถานะกระดาน 9 ช่อง เริ่มต้นทุกช่องเป็นค่าว่าง ""
scores = {"X": 0, "O": 0, "Draw": 0}
game_over = False # ตัวแปรเช็คว่าเกมจบหรือยัง (False = ยังไม่จบ, True = จบแล้ว)

WINS = [(0, 1, 2), (3, 4, 5), (6, 7, 8), # แนวนอน
        (0, 3, 6), (1, 4, 7), (2, 5, 8), # แนวตั้ง 
        (0, 4, 8), (2, 4, 6)] # แนวทแยง

def check_winner(b=None): # ฟังก์ชันตรวจสอบผู้ชนะ
    try:
        if b is None: # ถ้าไม่ส่งพารามิเตอร์ b มา
            state = board # ให้ใช้กระดานหลัก (board)
        else: # ถ้าส่ง b มา
            state = b # ให้ใช้ b แทน
        
        for a, b, c in WINS: # วนลูปตรวจสอบแต่ละแบบการชนะจากลิสต์ WINS
            if state[a] == state[b] == state[c] != "": # ถ้าตำแหน่ง a, b, c มีค่าเดียวกัน และไม่ใช่ค่าว่าง ""
                return state[a] # คืนค่าผู้ชนะ (X หรือ O)

        if "" not in state: # ถ้าไม่มีช่องว่างในกระดาน (เต็มหมด) แสดงว่าเสมอ
            return "Draw" # คืนค่า "Draw" (เสมอ)
        else: # ถ้ายังมีช่องว่าง และยังไม่มีผู้ชนะ
            return None # คืนค่า None (เกมยังไม่จบ)
    except Exception as e:
            # ถ้าเกิดข้อผิดพลาดในการตรวจสอบผู้ชนะ
            messagebox.showerror("ข้อผิดพลาด", f"เกิดข้อผิดพลาดในการตรวจสอบผู้ชนะ: {str(e)}")
            return None

def on_click(i): # ฟังก์ชันที่ทำงานเมื่อกดปุ่ม XO
    try:
        global current_player, game_over # ประกาศใช้ตัวแปร global เพื่อให้แก้ไขค่าได้

        if buttons[i]["text"] or game_over: # ถ้าปุ่มนี้มีข้อความอยู่แล้ว หรือเกมจบแล้ว
            return # หยุดการทำงานของฟังก์ชัน (ไม่ให้กดซ้ำ)

        buttons[i]["text"] = current_player # ใส่ตัวอักษร X หรือ O ลงในปุ่มที่กด

        if current_player == "X": # ถ้าผู้เล่นคนนี้เป็น X
            buttons[i]["bg"] = "#FFB6C1" # เปลี่ยนสีพื้นหลังปุ่มเป็นสีชมพู
        else: # ถ้าผู้เล่นคนนี้เป็น O
            buttons[i]["bg"] = "#87CEEB"  # เปลี่ยนสีพื้นหลังปุ่มเป็นสีฟ้า

        board[i] = current_player # บันทึกการเดินลงในลิสต์ board ที่ตำแหน่ง i
        winner = check_winner() # เรียกฟังก์ชันตรวจสอบว่ามีผู้ชนะหรือยัง

        if winner: # ถ้ามีผู้ชนะแล้ว (ไม่ว่าจะชนะหรือเสมอ)
            game_over = True # เปลี่ยนสถานะเกมเป็นจบ
            scores[winner] += 1 # เพิ่มคะแนนให้ผู้ชนะ (หรือเสมอ) 1 คะแนน

            if winner == "Draw": # ถ้าเป็นการเสมอ
                msg = "เกมเสมอ!" # สร้างข้อความ "เกมเสมอ!"
            else: # ถ้ามีผู้ชนะ
                msg = f"ผู้เล่น {winner} ชนะ!" # สร้างข้อความ "ผู้เล่น X ชนะ!" หรือ "ผู้เล่น O ชนะ!"

            messagebox.showinfo("ผลเกม", msg)# แสดงกล่องข้อความ popup พร้อมผลเกม
            status_label.config(text=msg) # อัปเดตข้อความสถานะเกมด้านล่าง
            update_score_display()# เรียกฟังก์ชันอัปเดตการแสดงผลคะแนน

        else:# ถ้ายังไม่มีผู้ชนะ (เกมยังไม่จบ)
            if current_player == "X": # ถ้าผู้เล่นคนนี้เป็น X
                # สลับเป็นตาของ O
                current_player = "O"# ถ้าผู้เล่นคนนี้เป็น O
            else:
                current_player = "X" # สลับเป็นตาของ X
            status_label.config(text=f"ถึงตา: {current_player}") # อัปเดตข้อความแสดงว่าถึงตาของผู้เล่นคนไหน
    except Exception as e:
            # ถ้าเกิดข้อผิดพลาดในการคลิกปุ่ม
            messagebox.showerror("ข้อผิดพลาด", f"เกิดข้อผิดพลาดในการเล่น: {str(e)}")

def reset_game(): # ฟังก์ชันรีเซ็ตเกมใหม่ (ไม่รีเซ็ตคะแนน) 
    try:
        # ประกาศใช้ตัวแปร global เพื่อให้แก้ไขค่าได้
        global current_player, board, game_over  # ประกาศใช้ตัวแปร global เพื่อให้แก้ไขค่าได้
        current_player, board, game_over = "X", [""] * 9, False  # รีเซ็ตค่าทั้งหมดกลับเป็นค่าเริ่มต้น  # current_player = "X", board = ["","","",...], game_over = False
        for btn in buttons: # วนลูปทุกปุ่มในลิสต์ buttons
            btn.config(text="", bg="#C79B99", state="normal")  # ล้างข้อความในปุ่ม, เปลี่ยนสีกลับเป็นสีเริ่มต้น, เปิดใช้งานปุ่ม
        status_label.config(text="ถึงตา: X") # แสดงข้อความ "ถึงตา: X" (เริ่มเกมใหม่)
    except Exception as e:
            # ถ้าเกิดข้อผิดพลาดในการอัปเดตคะแนน
        messagebox.showerror("ข้อผิดพลาด", f"เกิดข้อผิดพลาดในการอัปเดตคะแนน: {str(e)}")

def update_score_display(): # ฟังก์ชันอัปเดตการแสดงผลคะแนน
    try:
        # อัปเดต label แสดงคะแนนของ X, O และเสมอ
        score_label.config( text=f"คะแนน  X: {scores['X']}  |  O: {scores['O']}  |  เสมอ: {scores['Draw']}")
    except Exception as e:
            # ถ้าเกิดข้อผิดพลาดในการรีเซ็ตคะแนน
        messagebox.showerror("ข้อผิดพลาด", f"เกิดข้อผิดพลาดในการรีเซ็ตคะแนน: {str(e)}")
        
# ========== ส่วนของ UI (User Interface) ==========

mode_label = tk.Label(root, text="👥 สองผู้เล่น", font=("Arial", 18, "bold"), fg="blue")
mode_label.grid(row=0, column=0, columnspan=3, pady=15)

buttons = []
for i in range(9):
    # สร้างปุ่ม
    btn = tk.Button(root,  text="",  font=("Arial", 24, "bold"),  width=5,  height=2,  
                    command=lambda i=i: on_click(i),  bg="#C79B99")  
    # วางปุ่มในตาราง 3x3
    # i // 3 = หาแถว (0,1,2), i % 3 = หาคอลัมน์ (0,1,2)
    # +1 เพราะแถว 0 ใช้ไปแล้ว, เว้นระยะ 2 พิกเซล
    btn.grid(row=i // 3 + 1, column=i % 3, padx=2, pady=2)
    # เพิ่มปุ่มเข้าไปในลิสต์ buttons
    buttons.append(btn)

reset_button = tk.Button(root,  text="🔄 รีเซ็ตเกม",  font=("Arial", 12),  command=reset_game, bg="#87CEEB")  
reset_button.grid(row=4, column=0, columnspan=3, pady=10)

# สร้าง label แสดงสถานะเกม (ถึงตาใคร หรือใครชนะ)
status_label = tk.Label(root, text="ถึงตา: X", font=("Arial", 14, "bold"))
# วาง label ที่แถว 5 คอลัมน์ 0 แบบขยาย 3 คอลัมน์
status_label.grid(row=5, column=0, columnspan=3)

score_label = tk.Label(root,text="คะแนน  X: 0  |  O: 0  |  เสมอ: 0", font=("Arial", 12),  fg="green")  
score_label.grid(row=6, column=0, columnspan=3, pady=5)


reset_score_btn = tk.Button(root,text="🔄 รีเซ็ตคะแนน",font=("Arial", 12),command=lambda:
         (scores.update({"X": 0, "O": 0, "Draw": 0}), update_score_display()),bg="#FFA500", width=20) 
reset_score_btn.grid(row=7, column=0, columnspan=3, pady=10)

# เริ่มต้นโปรแกรม และรอรับการกระทำจากผู้ใช้ (เปิดหน้าต่าง)
root.mainloop()