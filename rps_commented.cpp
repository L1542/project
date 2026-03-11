#include <iostream>          // นำเข้า library สำหรับรับ/แสดงผลข้อมูล
#include <cstdlib>           // นำเข้า library สำหรับฟังก์ชัน rand() และ srand()
#include <ctime>             // นำเข้า library สำหรับฟังก์ชัน time() ใช้กับ srand
#include <fstream>           // นำเข้า library สำหรับอ่าน/เขียนไฟล์
#include <vector>            // นำเข้า library สำหรับใช้งาน vector (อาเรย์แบบไดนามิก)
#include <algorithm>         // นำเข้า library สำหรับฟังก์ชัน sort()
using namespace std;         // ใช้ namespace มาตรฐาน เพื่อไม่ต้องพิมพ์ std:: นำหน้า

struct RankEntry {           // กำหนด struct สำหรับเก็บข้อมูลคะแนนในตาราง ranking
    string name;             // ชื่อผู้เล่น
    int score;               // คะแนนของผู้เล่น
};

class Player {               // กำหนด class Player สำหรับผู้เล่นจริง
protected:                   // กำหนด access modifier เป็น protected (เข้าถึงได้จาก subclass)
    string name;             // ชื่อผู้เล่น (เข้าถึงได้จาก class ลูก)
public:                      // กำหนด access modifier เป็น public (เข้าถึงได้จากภายนอก)
    Player(string n);        // ประกาศ constructor รับชื่อผู้เล่น
    virtual int choose();    // ประกาศฟังก์ชัน virtual สำหรับให้ผู้เล่นเลือก (override ได้)
    int choose(int c);       // ประกาศฟังก์ชัน overload สำหรับส่งค่าตรงๆ โดยไม่ต้องรับ input
    string getName() { return name; } // ฟังก์ชัน getter คืนค่าชื่อผู้เล่น
};

Player::Player(string n) {   // นิยาม constructor ของ Player
    name = n;                // กำหนดชื่อผู้เล่นจาก parameter ที่รับมา
}

int Player::choose() {       // นิยามฟังก์ชัน choose() สำหรับรับ input จากผู้เล่น
    char c;                  // ตัวแปรสำหรับรับ input ของผู้เล่น
    while (true) {           // วนลูปไม่สิ้นสุดจนกว่าจะรับ input ที่ถูกต้อง
        cout << "Choose: 1 = Rock, 2 = Scissors, 3 = Paper (0 = Exit) : "; // แสดงตัวเลือก
        cin >> c;            // รับ input จากผู้ใช้

        if (cin.peek() != '\n') {       // ตรวจสอบว่ามีตัวอักษรเกินมาหลัง input หรือไม่
            cout << "Error: Please enter only ONE character\n"; // แจ้งข้อผิดพลาด
            cin.clear();                // ล้างสถานะ error ของ cin
            cin.ignore(1000, '\n');     // ลบ input ที่เหลือใน buffer ออก
            continue;                  // วนกลับไปรับ input ใหม่
        }

        if (c == '0') return 0;  // ถ้าเลือก 0 ให้ออกจากเกม
        if (c == '1') return 1;  // ถ้าเลือก 1 คือ Rock
        if (c == '2') return 2;  // ถ้าเลือก 2 คือ Scissors
        if (c == '3') return 3;  // ถ้าเลือก 3 คือ Paper

        cout << "Error: Number must be 0, 1, 2, or 3\n"; // แจ้ง error ถ้า input ไม่ถูกต้อง
    }
}

int Player::choose(int c) {  // นิยาม overloaded choose() รับค่าตรงๆ ไม่ต้องรับ input
    return c;                // คืนค่าที่รับมาเลย ใช้สำหรับบังคับค่าในบางรอบ
}

class Bot : public Player {  // กำหนด class Bot สืบทอดจาก Player
public:                      // กำหนด access modifier เป็น public
    Bot(string n) : Player(n) {} // constructor ส่งชื่อต่อให้ Player
    virtual int choose();         // ประกาศ virtual choose() ให้ Bot เลือกแบบสุ่ม
    int choose(int c) { return c; } // overload choose() คืนค่าที่กำหนดมาตรงๆ
};

int Bot::choose() {          // นิยามฟังก์ชัน choose() ของ Bot
    return rand() % 3 + 1;  // สุ่มตัวเลข 1-3 แทนการเลือกของบอท
}

class HardBot : public Bot { // กำหนด class HardBot สืบทอดจาก Bot (บอทฉลาดกว่า)
private:                     // กำหนด access modifier เป็น private
    int lastPlayerMove = 0;  // จำการเลือกรอบที่แล้วของผู้เล่น (0 = ยังไม่มีข้อมูล)
public:                      // กำหนด access modifier เป็น public
    HardBot(string n) : Bot(n) {}        // constructor ส่งชื่อต่อให้ Bot
    void rememberMove(int move) { lastPlayerMove = move; } // จำการเลือกของผู้เล่น
    int choose();            // ประกาศฟังก์ชัน choose() ที่ตอบโต้แบบฉลาด
};

int HardBot::choose() {      // นิยามฟังก์ชัน choose() ของ HardBot
    if (lastPlayerMove == 0) return rand() % 3 + 1; // รอบแรกยังไม่มีข้อมูล ให้สุ่ม
    if (lastPlayerMove == 1) return 3;  // ถ้าผู้เล่นเลือก Rock -> บอทเลือก Paper (ชนะ)
    if (lastPlayerMove == 2) return 1;  // ถ้าผู้เล่นเลือก Scissors -> บอทเลือก Rock (ชนะ)
    return 2;                           // ถ้าผู้เล่นเลือก Paper -> บอทเลือก Scissors (ชนะ)
}

class Rank {                 // กำหนด class Rank สำหรับจัดการตาราง ranking
private:                     // กำหนด access modifier เป็น private
    vector<RankEntry> rankList; // เก็บรายการ ranking ทั้งหมด
    string filename = "rank.txt"; // ชื่อไฟล์สำหรับบันทึกข้อมูล ranking
public:                      // กำหนด access modifier เป็น public
    void loadRank();         // ประกาศฟังก์ชันโหลด ranking จากไฟล์
    void saveRank();         // ประกาศฟังก์ชันบันทึก ranking ลงไฟล์
    void addScore(string name, int score); // ประกาศฟังก์ชันเพิ่ม/อัปเดตคะแนน
    void showRank();         // ประกาศฟังก์ชันแสดง ranking บนหน้าจอ
};

void Rank::loadRank() {      // นิยามฟังก์ชันโหลดข้อมูล ranking จากไฟล์
    ifstream file(filename); // เปิดไฟล์เพื่ออ่านข้อมูล
    if (!file.is_open()) return; // ถ้าเปิดไฟล์ไม่ได้ให้ออกจากฟังก์ชัน
    RankEntry entry;         // ตัวแปรชั่วคราวสำหรับเก็บแต่ละ entry
    while (file >> entry.name >> entry.score) { // อ่านชื่อและคะแนนจากไฟล์ทีละบรรทัด
        rankList.push_back(entry); // เพิ่ม entry ที่อ่านได้เข้า rankList
    }
    file.close();            // ปิดไฟล์หลังอ่านเสร็จ
}

void Rank::saveRank() {      // นิยามฟังก์ชันบันทึกข้อมูล ranking ลงไฟล์
    ofstream file(filename); // เปิดไฟล์เพื่อเขียนข้อมูล
    file << "================================\n"; // เขียนเส้นคั่น
    file << "         RANKING BOARD\n";           // เขียนหัวข้อ
    file << "================================\n"; // เขียนเส้นคั่น

    sort(rankList.begin(), rankList.end(), [](RankEntry a, RankEntry b) { // เรียงลำดับ rankList
        return a.score > b.score; // เรียงจากคะแนนมากไปน้อย (descending)
    });

    int place = 1;           // ตัวแปรนับลำดับ เริ่มที่ 1
    for (auto& e : rankList) { // วนลูปผ่านแต่ละ entry ใน rankList
        string medal = "";   // ตัวแปรเก็บเหรียญหรือลำดับ
        if (place == 1) medal = " [1st]";           // อันดับ 1
        else if (place == 2) medal = " [2nd]";      // อันดับ 2
        else if (place == 3) medal = " [3rd]";      // อันดับ 3
        else medal = " [" + to_string(place) + "th]"; // อันดับอื่นๆ

        file << medal << " " << e.name << " - " << e.score << " pts\n"; // เขียนข้อมูลลงไฟล์
        place++;             // เพิ่มลำดับ
    }

    if (rankList.empty()) file << "  No records yet.\n"; // ถ้าไม่มีข้อมูล เขียนแจ้งเตือน
    file << "================================\n"; // เขียนเส้นปิดท้าย
    file.close();            // ปิดไฟล์หลังเขียนเสร็จ
}

void Rank::addScore(string name, int score) { // นิยามฟังก์ชันเพิ่มหรืออัปเดตคะแนน
    for (auto& e : rankList) {  // วนหาชื่อที่มีอยู่แล้วใน rankList
        if (e.name == name) {   // ถ้าพบชื่อซ้ำ
            if (score > e.score) e.score = score; // อัปเดตคะแนนถ้าคะแนนใหม่สูงกว่า
            return;             // ออกจากฟังก์ชันหลังอัปเดต
        }
    }
    rankList.push_back({name, score}); // ถ้าไม่พบชื่อ เพิ่ม entry ใหม่เข้า rankList
}

void Rank::showRank() {      // นิยามฟังก์ชันแสดง ranking บนหน้าจอ
    sort(rankList.begin(), rankList.end(), [](RankEntry a, RankEntry b) { // เรียงลำดับ rankList
        return a.score > b.score; // เรียงจากคะแนนมากไปน้อย
    });
    cout << "\n===== RANKING =====\n"; // แสดงหัวข้อ ranking
    int place = 1;           // ตัวแปรนับลำดับ เริ่มที่ 1
    for (auto& e : rankList) { // วนลูปแสดงแต่ละ entry
        cout << place++ << ". " << e.name << " - " << e.score << " pts\n"; // แสดงลำดับ ชื่อ คะแนน
    }
    if (rankList.empty()) cout << "No records yet.\n"; // ถ้าไม่มีข้อมูล แจ้งเตือน
    cout << "===================\n"; // แสดงเส้นปิดท้าย
}

class Game {                 // กำหนด class Game สำหรับจัดการการเล่นแต่ละเกม
private:                     // กำหนด access modifier เป็น private
    Player*   player;        // pointer ชี้ไปยัง object ผู้เล่น
    Bot*      bot;           // pointer ชี้ไปยัง object บอท
    HardBot*  hardBot;       // pointer ชี้ไปยัง object HardBot (ถ้าเล่น hard mode)
    bool      isHard;        // flag บอกว่าเป็น hard mode หรือไม่
    int scorePlayer = 0;     // คะแนนสะสมของผู้เล่น เริ่มต้น 0
    int scoreBot    = 0;     // คะแนนสะสมของบอท เริ่มต้น 0
    bool exited     = false; // flag บอกว่าผู้เล่นกด exit ระหว่างเกมหรือไม่
public:                      // กำหนด access modifier เป็น public
    Game(Player* p, Bot* b, bool hard = false); // ประกาศ constructor รับ pointer ผู้เล่น บอท และโหมด
    void playRound(int round);  // ประกาศฟังก์ชันเล่นแต่ละรอบ
    void showResult(Rank& rank); // ประกาศฟังก์ชันแสดงผลสรุปเกม
    int  checkWin(int p, int b); // ประกาศฟังก์ชันตรวจสอบผู้ชนะแต่ละรอบ
    bool isExited() { return exited; } // getter คืนค่า flag exited
};

Game::Game(Player* p, Bot* b, bool hard) { // นิยาม constructor ของ Game
    player  = p;             // กำหนด pointer ผู้เล่น
    bot     = b;             // กำหนด pointer บอท
    isHard  = hard;          // กำหนดโหมดเกม
    hardBot = hard ? dynamic_cast<HardBot*>(b) : nullptr; // แปลง pointer เป็น HardBot ถ้าเป็น hard mode
}

string convertChoice(int c) { // ฟังก์ชันแปลงตัวเลขเป็นชื่อตัวเลือก
    if (c == 1) return "Rock";     // 1 = Rock
    if (c == 2) return "Scissors"; // 2 = Scissors
    if (c == 3) return "Paper";    // 3 = Paper
    return "";                     // ค่าอื่นคืนสตริงว่าง
}

void Game::playRound(int round) {  // นิยามฟังก์ชันเล่นแต่ละรอบ รับเลขรอบ
    cout << "\n----- Round " << round << " -----\n"; // แสดงหัวข้อรอบปัจจุบัน
    int p = player->choose(); // รับการเลือกจากผู้เล่น
    if (p == 0) {             // ถ้าผู้เล่นเลือก 0 แสดงว่าต้องการออกจากเกม
        cout << "You exited the game.\n"; // แจ้งว่าออกจากเกม
        exited = true;        // ตั้ง flag exited เป็น true
        return;               // ออกจากฟังก์ชัน
    }

    int b = (round == 1) ? bot->choose(1) : bot->choose(); // รอบแรกบอทเลือก Rock เสมอ, รอบต่อไปสุ่ม

    cout << "You chose  : " << convertChoice(p) << "\n"; // แสดงการเลือกของผู้เล่น
    cout << "Bot chose  : " << convertChoice(b) << "\n"; // แสดงการเลือกของบอท

    int result = checkWin(p, b); // ตรวจสอบว่าใครชนะรอบนี้
    if (result == 1) {           // ถ้าผู้เล่นชนะ
        cout << "You win this round!\n"; // แจ้งว่าผู้เล่นชนะ
        scorePlayer++;           // เพิ่มคะแนนผู้เล่น
    } else if (result == 2) {    // ถ้าบอทชนะ
        cout << "Bot wins this round!\n"; // แจ้งว่าบอทชนะ
        scoreBot++;              // เพิ่มคะแนนบอท
    } else {                     // ถ้าเสมอ
        cout << "Draw!\n";       // แจ้งว่าเสมอ
    }
    if (isHard && hardBot != nullptr) { // ถ้าเป็น hard mode และ hardBot ถูกตั้งค่าแล้ว
        hardBot->rememberMove(p);       // บอกให้ HardBot จำการเลือกของผู้เล่นในรอบนี้
    }
}

int Game::checkWin(int p, int b) { // นิยามฟังก์ชันตรวจสอบผู้ชนะ
    if (p == b) return 0;          // ถ้าเลือกเหมือนกัน คือเสมอ
    if ((p == 1 && b == 2) ||      // Rock ชนะ Scissors
        (p == 2 && b == 3) ||      // Scissors ชนะ Paper
        (p == 3 && b == 1))        // Paper ชนะ Rock
        return 1;                  // ผู้เล่นชนะ คืนค่า 1
    return 2;                      // บอทชนะ คืนค่า 2
}

void Game::showResult(Rank& rank) { // นิยามฟังก์ชันแสดงผลสรุปเกมและอัปเดต ranking
    cout << "\n===== FINAL RESULT =====\n"; // แสดงหัวข้อผลสรุป
    cout << "Player Score : " << scorePlayer << "\n"; // แสดงคะแนนผู้เล่น
    cout << "Bot Score    : " << scoreBot    << "\n"; // แสดงคะแนนบอท

    if (scorePlayer > scoreBot)       // ถ้าผู้เล่นคะแนนมากกว่า
        cout << "You win the game!\n";  // แจ้งว่าผู้เล่นชนะ
    else if (scoreBot > scorePlayer)  // ถ้าบอทคะแนนมากกว่า
        cout << "Bot wins the game!\n"; // แจ้งว่าบอทชนะ
    else                              // ถ้าคะแนนเท่ากัน
        cout << "Game Draw!\n";         // แจ้งว่าเสมอ

    rank.addScore(player->getName(), scorePlayer); // เพิ่ม/อัปเดตคะแนนของผู้เล่นใน ranking
    rank.saveRank();  // บันทึก ranking ลงไฟล์
    rank.showRank();  // แสดง ranking บนหน้าจอ
}

int main() {                 // ฟังก์ชันหลักของโปรแกรม
    srand(time(0));          // ตั้งค่า seed สำหรับการสุ่มโดยใช้เวลาปัจจุบัน

    Rank rank;               // สร้าง object rank สำหรับจัดการ ranking
    rank.loadRank();         // โหลดข้อมูล ranking จากไฟล์ (ถ้ามี)

    while (true) {           // วนลูปแสดงเมนูหลักจนกว่าจะเลือกออก
        cout << "\n===== MENU =====\n";           // แสดงหัวข้อเมนู
        cout << "1. Play Game (Normal Bot)\n";    // เลือกเล่นกับบอทปกติ
        cout << "2. Play Game (Hard Bot)\n";      // เลือกเล่นกับบอทยาก
        cout << "3. Show Ranking\n";              // เลือกดู ranking
        cout << "4. Exit\n";                      // เลือกออกจากโปรแกรม
        cout << "Choose: ";                       // แสดง prompt รับ input

        char menu;           // ตัวแปรรับการเลือกเมนู
        cin >> menu;         // รับ input จากผู้ใช้

        if (cin.peek() != '\n') {        // ตรวจสอบว่ามีตัวอักษรเกินมาหรือไม่
            cin.clear();                 // ล้างสถานะ error ของ cin
            cin.ignore(1000, '\n');      // ลบ input ที่เหลือใน buffer
            cout << "Error: Please enter only ONE character\n"; // แจ้ง error
            continue;                   // วนกลับไปแสดงเมนูใหม่
        }

        if (menu == '4') {               // ถ้าเลือก 4 ออกจากโปรแกรม
            cout << "Goodbye!\n";        // แสดงข้อความลา
            break;                       // หยุดลูปเมนูหลัก
        } else if (menu == '3') {        // ถ้าเลือก 3 ดู ranking
            rank.showRank();             // แสดง ranking
        } else if (menu == '1' || menu == '2') { // ถ้าเลือก 1 หรือ 2 เริ่มเล่นเกม
            string playerName;           // ตัวแปรเก็บชื่อผู้เล่น
            cout << "Enter your name: "; // ขอชื่อผู้เล่น
            cin >> playerName;           // รับชื่อจาก input
            cin.ignore(1000, '\n');      // ล้าง newline ใน buffer

            Player p(playerName);        // สร้าง object ผู้เล่นด้วยชื่อที่รับมา

            if (menu == '2') {           // ถ้าเลือก hard mode
                HardBot hb("HardBot");   // สร้าง object HardBot
                Game game(&p, &hb, true); // สร้างเกมโดยส่ง pointer ผู้เล่น HardBot และ hard=true
                cout << "[Hard Mode] Bot will counter your previous move!\n"; // แจ้งโหมด hard
                for (int i = 1; i <= 3; i++) { // วนเล่น 3 รอบ
                    game.playRound(i);           // เล่นรอบที่ i
                    if (game.isExited()) break;  // ถ้าผู้เล่นกด exit ให้หยุด
                }
                if (!game.isExited()) game.showResult(rank); // ถ้าไม่ได้ exit ให้แสดงผลสรุป
            } else {                     // ถ้าเลือก normal mode
                Bot b("Bot");            // สร้าง object Bot ปกติ
                Game game(&p, &b, false); // สร้างเกมโดยส่ง pointer ผู้เล่น Bot และ hard=false
                for (int i = 1; i <= 3; i++) { // วนเล่น 3 รอบ
                    game.playRound(i);           // เล่นรอบที่ i
                    if (game.isExited()) break;  // ถ้าผู้เล่นกด exit ให้หยุด
                }
                if (!game.isExited()) game.showResult(rank); // ถ้าไม่ได้ exit ให้แสดงผลสรุป
            }
        }
    }

    return 0;                // จบการทำงานของโปรแกรม คืนค่า 0 (สำเร็จ)
}
