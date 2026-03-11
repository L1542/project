#include <iostream>          // นำเข้า library iostream สำหรับใช้คำสั่ง cout (แสดงผล) และ cin (รับข้อมูล) จากผู้ใช้
#include <cstdlib>           // นำเข้า library cstdlib สำหรับใช้ฟังก์ชัน rand() (สุ่มตัวเลข) และ srand() (ตั้ง seed การสุ่ม)
#include <ctime>             // นำเข้า library ctime สำหรับใช้ฟังก์ชัน time() เพื่อเอาเวลาปัจจุบันมาเป็น seed ให้ rand()
#include <fstream>           // นำเข้า library fstream สำหรับใช้ ifstream (อ่านไฟล์) และ ofstream (เขียนไฟล์)
#include <vector>            // นำเข้า library vector สำหรับใช้ vector<> ซึ่งเป็นอาเรย์แบบไดนามิก ปรับขนาดได้อัตโนมัติ
#include <algorithm>         // นำเข้า library algorithm สำหรับใช้ฟังก์ชัน sort() เพื่อเรียงลำดับข้อมูลใน vector
using namespace std;         // ประกาศใช้ namespace มาตรฐาน ทำให้ไม่ต้องพิมพ์ std:: นำหน้าทุกครั้ง เช่น std::cout → cout

struct RankEntry {           // ประกาศ struct ชื่อ RankEntry สำหรับเก็บข้อมูล 1 รายการใน ranking board (ชื่อ + คะแนน)
    string name;             // ตัวแปรสำหรับเก็บชื่อผู้เล่นในรายการ ranking
    int score;               // ตัวแปรสำหรับเก็บคะแนนสูงสุดของผู้เล่นในรายการ ranking
};

class Player {               // ประกาศ class Player เป็น base class หลักสำหรับผู้เล่นจริง (มนุษย์ที่นั่งเล่นอยู่)
protected:                   // access modifier แบบ protected หมายถึงตัวแปรนี้เข้าถึงได้จาก class นี้เองและ class ลูก (Bot, HardBot)
    string name;             // ตัวแปรเก็บชื่อผู้เล่น ถูกทำให้ protected เพื่อให้ class ลูกใช้ชื่อผ่าน inheritance ได้
public:                      // access modifier แบบ public หมายถึงทุก class และโค้ดภายนอกสามารถเรียกใช้ได้
    Player(string n);        // ประกาศ constructor รับ parameter ชื่อ n เพื่อกำหนดชื่อผู้เล่นตอนสร้าง object
    virtual int choose();    // ประกาศ virtual function choose() ไม่รับ parameter ใช้สำหรับรับ input จากผู้เล่น และ virtual ทำให้ class ลูก override ได้
    int choose(int c);       // ประกาศ overloaded function choose(int c) รับ parameter c เพื่อกำหนดค่าตรงๆ โดยไม่ต้องรับ input (ใช้ใน playRound รอบแรก)
    string getName() { return name; } // ฟังก์ชัน getter แบบ inline คืนค่าชื่อผู้เล่น ใช้เพื่อดึงชื่อไปบันทึก ranking
};

Player::Player(string n) {   // นิยาม constructor ของ class Player โดยใช้ Scope Resolution Operator (::) เพื่อระบุว่าเป็น constructor ของ Player
    name = n;                // กำหนดค่าตัวแปร name ของ object ด้วยค่า n ที่รับมาจาก parameter
}

int Player::choose() {       // นิยามฟังก์ชัน choose() ของ Player ใช้ :: บอกว่าเป็นของ Player และทำหน้าที่รับ input จากผู้เล่นจริง
    char c;                  // ประกาศตัวแปร c ชนิด char สำหรับรับตัวอักษรที่ผู้เล่นพิมพ์เข้ามา
    while (true) {           // วนลูปแบบไม่มีเงื่อนไขสิ้นสุด (infinite loop) เพื่อรับ input ซ้ำจนกว่าจะได้ค่าที่ถูกต้อง
        cout << "Choose: 1 = Rock, 2 = Scissors, 3 = Paper (0 = Exit) : "; // แสดงตัวเลือกให้ผู้เล่นเห็นก่อนรับ input
        cin >> c;            // รับตัวอักษร 1 ตัวจากผู้ใช้เก็บไว้ในตัวแปร c

        if (cin.peek() != '\n') {       // ตรวจสอบว่าตัวอักษรถัดไปใน input buffer คือ newline หรือไม่ ถ้าไม่ใช่แปลว่าพิมพ์มาหลายตัว
            cout << "Error: Please enter only ONE character\n"; // แสดงข้อความแจ้งข้อผิดพลาดว่าพิมพ์มาเกิน 1 ตัว
            cin.clear();                // ล้างสถานะ error flag ของ cin เพื่อให้รับ input ได้ปกติในรอบถัดไป
            cin.ignore(1000, '\n');     // ลบตัวอักษรที่เหลือใน buffer ทิ้งสูงสุด 1000 ตัว หรือจนถึง newline
            continue;                  // ข้ามไปเริ่มรอบใหม่ของ while loop เพื่อรับ input ใหม่
        }

        if (c == '0') return 0;  // ถ้าผู้เล่นพิมพ์ '0' ให้คืนค่า 0 ซึ่งหมายถึงต้องการออกจากเกม (Exit)
        if (c == '1') return 1;  // ถ้าผู้เล่นพิมพ์ '1' ให้คืนค่า 1 หมายถึงเลือก Rock (กรรไกรหิน)
        if (c == '2') return 2;  // ถ้าผู้เล่นพิมพ์ '2' ให้คืนค่า 2 หมายถึงเลือก Scissors (กรรไกร)
        if (c == '3') return 3;  // ถ้าผู้เล่นพิมพ์ '3' ให้คืนค่า 3 หมายถึงเลือก Paper (กระดาษ)

        cout << "Error: Number must be 0, 1, 2, or 3\n"; // ถ้าพิมพ์ตัวอักษรอื่นนอกจาก 0-3 แสดงข้อความแจ้งข้อผิดพลาด
    }
}

int Player::choose(int c) {  // นิยาม overloaded function choose(int c) รับค่าตรงๆ ไม่ต้องรับ input ใช้ส่งค่าที่กำหนดไว้ล่วงหน้า
    return c;                // คืนค่า c ที่รับมาออกไปเลย เช่น choose(1) คืนค่า 1 = Rock โดยไม่ต้องรอ input
}

class Bot : public Player {  // ประกาศ class Bot สืบทอด (inherit) จาก Player แบบ public ทำให้ Bot มีทุกอย่างของ Player
public:                      // access modifier แบบ public
    Bot(string n) : Player(n) {} // constructor ของ Bot เรียก constructor ของ Player ด้วย initializer list เพื่อกำหนดชื่อ
    virtual int choose();         // ประกาศ virtual choose() ไม่รับ parameter สำหรับให้บอทสุ่มตัวเลข และ virtual ให้ HardBot override ได้
    int choose(int c) { return c; } // overloaded choose(int c) คืนค่าที่รับมาตรงๆ เหมือน Player ใช้กรณีบังคับให้บอทเลือกค่าที่กำหนด
};

int Bot::choose() {          // นิยามฟังก์ชัน choose() ของ Bot ใช้ :: บอกว่าเป็นของ Bot ทำหน้าที่สุ่มตัวเลือก
    return rand() % 3 + 1;  // สุ่มตัวเลข 0-2 ด้วย rand()%3 แล้วบวก 1 ได้ผลลัพธ์เป็น 1, 2 หรือ 3 (Rock, Scissors, Paper)
}

class HardBot : public Bot { // ประกาศ class HardBot สืบทอดจาก Bot (multilevel: HardBot→Bot→Player) เป็นบอทฉลาดที่จำการเลือกของผู้เล่น
private:                     // access modifier แบบ private หมายถึงเข้าถึงได้จาก class HardBot เท่านั้น
    int lastPlayerMove = 0;  // ตัวแปรจำการเลือกล่าสุดของผู้เล่น กำหนดค่าเริ่มต้นเป็น 0 หมายถึงยังไม่มีข้อมูล (รอบแรก)
public:                      // access modifier แบบ public
    HardBot(string n) : Bot(n) {}        // constructor ส่งชื่อต่อให้ Bot ผ่าน initializer list
    void rememberMove(int move) { lastPlayerMove = move; } // ฟังก์ชัน setter รับการเลือกของผู้เล่นมาเก็บไว้ใน lastPlayerMove
    int choose();            // ประกาศฟังก์ชัน choose() ที่ override จาก Bot เพื่อตอบโต้แบบฉลาดโดยใช้ lastPlayerMove
};

int HardBot::choose() {      // นิยามฟังก์ชัน choose() ของ HardBot ใช้ :: บอกว่าเป็นของ HardBot เลือกโดยดูจากการเลือกรอบที่แล้วของผู้เล่น
    if (lastPlayerMove == 0) return rand() % 3 + 1; // รอบแรก lastPlayerMove = 0 ยังไม่รู้แนวโน้ม ให้สุ่มเหมือน Bot ปกติ
    if (lastPlayerMove == 1) return 3;  // ถ้าผู้เล่นเลือก Rock (1) รอบที่แล้ว บอทเลือก Paper (3) เพื่อชนะ
    if (lastPlayerMove == 2) return 1;  // ถ้าผู้เล่นเลือก Scissors (2) รอบที่แล้ว บอทเลือก Rock (1) เพื่อชนะ
    return 2;                           // ถ้าผู้เล่นเลือก Paper (3) รอบที่แล้ว บอทเลือก Scissors (2) เพื่อชนะ
}

class Rank {                 // ประกาศ class Rank สำหรับจัดการระบบ ranking ทั้งหมด ทั้งบันทึก โหลด และแสดงผล
private:                     // access modifier แบบ private ทำให้ข้อมูลและไฟล์เข้าถึงได้จาก class Rank เท่านั้น
    vector<RankEntry> rankList; // ตัวแปร vector เก็บรายการ RankEntry ทั้งหมด (ชื่อ+คะแนน) ปรับขนาดได้อัตโนมัติ
    string filename = "rank.txt"; // ตัวแปรเก็บชื่อไฟล์ที่ใช้บันทึก ranking กำหนดเป็น "rank.txt" ไว้ตั้งแต่ต้น
public:                      // access modifier แบบ public ให้โค้ดภายนอกเรียกฟังก์ชันเหล่านี้ได้
    void loadRank();         // ประกาศฟังก์ชัน loadRank() สำหรับโหลดข้อมูล ranking จากไฟล์ rank.txt มาเก็บใน rankList
    void saveRank();         // ประกาศฟังก์ชัน saveRank() สำหรับบันทึกข้อมูลใน rankList ลงไฟล์ rank.txt พร้อม format สวยงาม
    void addScore(string name, int score); // ประกาศฟังก์ชัน addScore() สำหรับเพิ่มคะแนนใหม่หรืออัปเดตถ้าชื่อซ้ำ
    void showRank();         // ประกาศฟังก์ชัน showRank() สำหรับแสดงตาราง ranking บนหน้าจอ console
};

void Rank::loadRank() {      // นิยามฟังก์ชัน loadRank() ของ Rank ใช้ :: บอกว่าเป็นของ Rank อ่านข้อมูลจากไฟล์เข้า rankList
    ifstream file(filename); // สร้าง object ifstream ชื่อ file และเปิดไฟล์ rank.txt เพื่ออ่าน (input file stream)
    if (!file.is_open()) return; // ตรวจสอบว่าเปิดไฟล์ได้หรือไม่ ถ้าเปิดไม่ได้ (ไฟล์ไม่มี) ให้ออกจากฟังก์ชันทันที
    RankEntry entry;         // ประกาศตัวแปร entry ชนิด RankEntry เป็นตัวแปรชั่วคราวสำหรับรับข้อมูลแต่ละบรรทัดจากไฟล์
    while (file >> entry.name >> entry.score) { // อ่านชื่อและคะแนนจากไฟล์ทีละคู่ วนซ้ำจนหมดไฟล์ (ถ้าอ่านไม่ได้ loop จบ)
        rankList.push_back(entry); // เพิ่ม entry ที่อ่านได้เข้าท้าย rankList โดยใช้ push_back ของ vector
    }
    file.close();            // ปิดไฟล์หลังอ่านข้อมูลครบ เพื่อคืน resource ให้ระบบ
}

void Rank::saveRank() {      // นิยามฟังก์ชัน saveRank() ของ Rank ใช้ :: บอกว่าเป็นของ Rank เขียนข้อมูลทั้งหมดลงไฟล์
    ofstream file(filename); // สร้าง object ofstream ชื่อ file และเปิดไฟล์ rank.txt เพื่อเขียน (output file stream) ทับของเดิม
    file << "================================\n"; // เขียนเส้นคั่นบนสุดเพื่อความสวยงาม
    file << "         RANKING BOARD\n";           // เขียนหัวข้อ RANKING BOARD ตรงกลาง
    file << "================================\n"; // เขียนเส้นคั่นใต้หัวข้อ

    sort(rankList.begin(), rankList.end(), [](RankEntry a, RankEntry b) { // เรียงลำดับ rankList โดยใช้ sort() กับ lambda function
        return a.score > b.score; // lambda บอกว่าให้เรียง a ก่อน b ถ้า a.score มากกว่า b.score (descending = มากไปน้อย)
    });

    int place = 1;           // ประกาศตัวแปร place เก็บลำดับปัจจุบัน เริ่มที่ 1 (อันดับ 1)
    for (auto& e : rankList) { // วนลูปผ่านทุก entry ใน rankList โดย auto& e คือ reference ไปยังแต่ละ RankEntry
        string medal = "";   // ประกาศตัวแปร medal เก็บข้อความแสดงลำดับ เริ่มต้นเป็นสตริงว่าง
        if (place == 1) medal = " [1st]";           // ถ้าลำดับที่ 1 ใช้ป้าย [1st]
        else if (place == 2) medal = " [2nd]";      // ถ้าลำดับที่ 2 ใช้ป้าย [2nd]
        else if (place == 3) medal = " [3rd]";      // ถ้าลำดับที่ 3 ใช้ป้าย [3rd]
        else medal = " [" + to_string(place) + "th]"; // ลำดับที่ 4 ขึ้นไป แปลง place เป็น string แล้วต่อกับ "th]"

        file << medal << " " << e.name << " - " << e.score << " pts\n"; // เขียนลำดับ ชื่อ และคะแนนลงไฟล์
        place++;             // เพิ่มค่า place ขึ้น 1 เพื่อไปลำดับถัดไป
    }

    if (rankList.empty()) file << "  No records yet.\n"; // ถ้า rankList ว่างเปล่า เขียนข้อความแจ้งว่ายังไม่มีข้อมูล
    file << "================================\n"; // เขียนเส้นคั่นปิดท้ายไฟล์
    file.close();            // ปิดไฟล์หลังเขียนข้อมูลครบ เพื่อ flush ข้อมูลและคืน resource
}

void Rank::addScore(string name, int score) { // นิยามฟังก์ชัน addScore() รับชื่อและคะแนน เพื่อเพิ่มหรืออัปเดตข้อมูลใน rankList
    for (auto& e : rankList) {  // วนหาใน rankList ว่ามีชื่อนี้อยู่แล้วหรือไม่
        if (e.name == name) {   // ถ้าพบชื่อซ้ำใน rankList แสดงว่าผู้เล่นคนนี้เคยเล่นมาก่อน
            if (score > e.score) e.score = score; // อัปเดตคะแนนเฉพาะเมื่อคะแนนใหม่สูงกว่าของเดิม (เก็บ best score)
            return;             // ออกจากฟังก์ชันหลังอัปเดตคะแนนแล้ว ไม่ต้องเพิ่ม entry ใหม่
        }
    }
    rankList.push_back({name, score}); // ถ้าไม่พบชื่อในลูป แสดงว่าผู้เล่นใหม่ ให้เพิ่ม entry ใหม่เข้า rankList
}

void Rank::showRank() {      // นิยามฟังก์ชัน showRank() ของ Rank แสดงตาราง ranking บนหน้าจอ console
    sort(rankList.begin(), rankList.end(), [](RankEntry a, RankEntry b) { // เรียงลำดับ rankList ก่อนแสดงผล
        return a.score > b.score; // เรียงจากคะแนนมากไปน้อย เหมือนกับใน saveRank
    });
    cout << "\n===== RANKING =====\n"; // แสดงหัวข้อ RANKING บนหน้าจอ
    int place = 1;           // ตัวแปรนับลำดับ เริ่มที่ 1
    for (auto& e : rankList) { // วนลูปแสดงแต่ละ entry ใน rankList
        cout << place++ << ". " << e.name << " - " << e.score << " pts\n"; // แสดงลำดับ (แล้วเพิ่ม place++) ชื่อ และคะแนน
    }
    if (rankList.empty()) cout << "No records yet.\n"; // ถ้า rankList ว่าง แสดงข้อความแจ้งว่ายังไม่มีข้อมูล
    cout << "===================\n"; // แสดงเส้นปิดท้ายตาราง
}

class Game {                 // ประกาศ class Game สำหรับควบคุมการเล่นทั้งหมด ตั้งแต่ต้นจนจบเกม
private:                     // access modifier แบบ private ให้ข้อมูลภายในเกมเข้าถึงได้จาก class Game เท่านั้น
    Player*   player;        // pointer ชี้ไปยัง object ของผู้เล่น (ใช้ pointer เพื่อรองรับ polymorphism)
    Bot*      bot;           // pointer ชี้ไปยัง object ของบอท (ใช้ pointer เพื่อรองรับทั้ง Bot และ HardBot)
    HardBot*  hardBot;       // pointer ชี้ไปยัง object HardBot โดยเฉพาะ ใช้เรียก rememberMove() ซึ่งไม่มีใน Bot
    bool      isHard;        // flag เก็บว่าเกมนี้เป็น hard mode หรือไม่ ใช้ตัดสินใจใน playRound
    int scorePlayer = 0;     // ตัวแปรสะสมคะแนนของผู้เล่น กำหนดค่าเริ่มต้นเป็น 0
    int scoreBot    = 0;     // ตัวแปรสะสมคะแนนของบอท กำหนดค่าเริ่มต้นเป็น 0
    bool exited     = false; // flag บอกว่าผู้เล่นกด exit (0) ระหว่างเกมหรือไม่ ใช้หยุด loop ใน main
public:                      // access modifier แบบ public
    Game(Player* p, Bot* b, bool hard = false); // ประกาศ constructor รับ pointer ผู้เล่น pointer บอท และโหมด (default = false)
    void playRound(int round);  // ประกาศฟังก์ชันเล่น 1 รอบ รับเลขรอบเป็น parameter
    void showResult(Rank& rank); // ประกาศฟังก์ชันแสดงผลสรุปเกมและบันทึก ranking รับ reference ของ Rank
    int  checkWin(int p, int b); // ประกาศฟังก์ชันตรวจสอบผู้ชนะโดยเปรียบเทียบการเลือกของผู้เล่นและบอท
    bool isExited() { return exited; } // getter แบบ inline คืนค่า flag exited ใช้ตรวจสอบใน main ว่าผู้เล่นออกหรือไม่
};

Game::Game(Player* p, Bot* b, bool hard) { // นิยาม constructor ของ Game ใช้ :: บอกว่าเป็นของ Game
    player  = p;             // กำหนด pointer player ให้ชี้ไปยัง object ผู้เล่นที่รับมา
    bot     = b;             // กำหนด pointer bot ให้ชี้ไปยัง object บอทที่รับมา
    isHard  = hard;          // กำหนด flag isHard ตามค่า hard ที่รับมา
    hardBot = hard ? dynamic_cast<HardBot*>(b) : nullptr; // ถ้า hard mode แปลง pointer b เป็น HardBot* ด้วย dynamic_cast ถ้าไม่ใช่ hard ให้เป็น nullptr
}

string convertChoice(int c) { // ฟังก์ชัน helper แปลงตัวเลข 1-3 เป็นชื่อตัวเลือกเพื่อแสดงผลบนหน้าจอ
    if (c == 1) return "Rock";     // เลข 1 แทน Rock (หิน)
    if (c == 2) return "Scissors"; // เลข 2 แทน Scissors (กรรไกร)
    if (c == 3) return "Paper";    // เลข 3 แทน Paper (กระดาษ)
    return "";                     // ถ้าได้ค่าอื่น (ไม่ควรเกิดขึ้น) คืนสตริงว่าง
}

void Game::playRound(int round) {  // นิยามฟังก์ชัน playRound() ของ Game รับเลขรอบเพื่อแสดงหัวข้อ
    cout << "\n----- Round " << round << " -----\n"; // แสดงหัวข้อรอบปัจจุบัน เช่น "----- Round 1 -----"
    int p = player->choose(); // เรียก choose() ของผู้เล่นผ่าน pointer รับผลการเลือกเก็บใน p
    if (p == 0) {             // ตรวจสอบว่าผู้เล่นเลือก 0 (Exit) หรือไม่
        cout << "You exited the game.\n"; // แสดงข้อความแจ้งว่าผู้เล่นออกจากเกม
        exited = true;        // ตั้ง flag exited เป็น true เพื่อให้ main loop รู้ว่าต้องหยุด
        return;               // ออกจากฟังก์ชัน playRound ทันทีโดยไม่เล่นต่อ
    }

    int b = (round == 1) ? bot->choose(1) : bot->choose(); // รอบที่ 1 บังคับให้บอทเลือก Rock(1) รอบถัดไปให้สุ่มหรือใช้ logic ของ HardBot

    cout << "You chose  : " << convertChoice(p) << "\n"; // แสดงการเลือกของผู้เล่นโดยแปลงเลขเป็นชื่อด้วย convertChoice
    cout << "Bot chose  : " << convertChoice(b) << "\n"; // แสดงการเลือกของบอทโดยแปลงเลขเป็นชื่อด้วย convertChoice

    int result = checkWin(p, b); // เรียก checkWin เปรียบเทียบการเลือกของทั้งคู่ ได้ผลลัพธ์ 0=เสมอ 1=ผู้เล่นชนะ 2=บอทชนะ
    if (result == 1) {           // ถ้า result เป็น 1 แสดงว่าผู้เล่นชนะรอบนี้
        cout << "You win this round!\n"; // แสดงข้อความแจ้งว่าผู้เล่นชนะ
        scorePlayer++;           // เพิ่มคะแนนสะสมของผู้เล่น 1 แต้ม
    } else if (result == 2) {    // ถ้า result เป็น 2 แสดงว่าบอทชนะรอบนี้
        cout << "Bot wins this round!\n"; // แสดงข้อความแจ้งว่าบอทชนะ
        scoreBot++;              // เพิ่มคะแนนสะสมของบอท 1 แต้ม
    } else {                     // ถ้า result เป็น 0 แสดงว่าเสมอกัน
        cout << "Draw!\n";       // แสดงข้อความแจ้งว่าเสมอ ไม่มีใครได้คะแนน
    }
    if (isHard && hardBot != nullptr) { // ตรวจสอบว่าเป็น hard mode และ hardBot ไม่ใช่ nullptr (ถูก cast สำเร็จ)
        hardBot->rememberMove(p);       // เรียก rememberMove() ส่งการเลือกของผู้เล่นรอบนี้ให้ HardBot จำไว้ใช้รอบถัดไป
    }
}

int Game::checkWin(int p, int b) { // นิยามฟังก์ชัน checkWin() ของ Game รับการเลือกของผู้เล่น p และบอท b
    if (p == b) return 0;          // ถ้าเลือกเหมือนกัน คืน 0 หมายถึงเสมอ
    if ((p == 1 && b == 2) ||      // Rock (1) ชนะ Scissors (2)
        (p == 2 && b == 3) ||      // Scissors (2) ชนะ Paper (3)
        (p == 3 && b == 1))        // Paper (3) ชนะ Rock (1)
        return 1;                  // ถ้าเข้าเงื่อนไขใดเงื่อนไขหนึ่ง ผู้เล่นชนะ คืน 1
    return 2;                      // ถ้าไม่เสมอและผู้เล่นไม่ชนะ แปลว่าบอทชนะ คืน 2
}

void Game::showResult(Rank& rank) { // นิยามฟังก์ชัน showResult() รับ reference ของ Rank เพื่ออัปเดตและแสดง ranking
    cout << "\n===== FINAL RESULT =====\n"; // แสดงหัวข้อผลสรุปเกม
    cout << "Player Score : " << scorePlayer << "\n"; // แสดงคะแนนรวมของผู้เล่นตลอดเกม
    cout << "Bot Score    : " << scoreBot    << "\n"; // แสดงคะแนนรวมของบอทตลอดเกม

    if (scorePlayer > scoreBot)       // ถ้าคะแนนผู้เล่นมากกว่าคะแนนบอท
        cout << "You win the game!\n";  // แสดงข้อความว่าผู้เล่นชนะทั้งเกม
    else if (scoreBot > scorePlayer)  // ถ้าคะแนนบอทมากกว่าคะแนนผู้เล่น
        cout << "Bot wins the game!\n"; // แสดงข้อความว่าบอทชนะทั้งเกม
    else                              // ถ้าคะแนนเท่ากัน
        cout << "Game Draw!\n";         // แสดงข้อความว่าเกมเสมอ

    rank.addScore(player->getName(), scorePlayer); // ดึงชื่อผู้เล่นด้วย getName() แล้วเรียก addScore() เพื่อบันทึกคะแนนใน ranking
    rank.saveRank();  // บันทึก ranking ที่อัปเดตแล้วลงไฟล์ rank.txt
    rank.showRank();  // แสดง ranking บนหน้าจอ console ให้ผู้เล่นเห็น
}

int main() {                 // ฟังก์ชัน main() เป็นจุดเริ่มต้นการทำงานของโปรแกรม
    srand(time(0));          // ตั้งค่า seed การสุ่มด้วยเวลาปัจจุบัน (วินาที) ทำให้การสุ่มแตกต่างกันทุกครั้งที่รันโปรแกรม

    Rank rank;               // สร้าง object rank ชนิด Rank สำหรับจัดการ ranking ตลอดการทำงานของโปรแกรม
    rank.loadRank();         // โหลดข้อมูล ranking จากไฟล์ rank.txt (ถ้าไม่มีไฟล์ฟังก์ชันจะ return ออกมาเฉยๆ)

    while (true) {           // วนลูปหลักของโปรแกรม แสดงเมนูซ้ำไปเรื่อยๆ จนกว่าผู้ใช้จะเลือก Exit (4)
        cout << "\n===== MENU =====\n";           // แสดงหัวข้อเมนูหลัก
        cout << "1. Play Game (Normal Bot)\n";    // แสดงตัวเลือก 1 เล่นกับ Bot ปกติที่สุ่มตัวเลือก
        cout << "2. Play Game (Hard Bot)\n";      // แสดงตัวเลือก 2 เล่นกับ HardBot ที่จำการเลือกของผู้เล่น
        cout << "3. Show Ranking\n";              // แสดงตัวเลือก 3 ดูตาราง ranking
        cout << "4. Exit\n";                      // แสดงตัวเลือก 4 ออกจากโปรแกรม
        cout << "Choose: ";                       // แสดง prompt ชวนให้ผู้ใช้พิมพ์ตัวเลือก

        char menu;           // ประกาศตัวแปร menu ชนิด char สำหรับรับการเลือกเมนูจากผู้ใช้
        cin >> menu;         // รับตัวอักษร 1 ตัวจากผู้ใช้เก็บใน menu

        if (cin.peek() != '\n') {        // ตรวจสอบว่ามีตัวอักษรเกินมาใน buffer หรือไม่ เพื่อป้องกันการพิมพ์หลายตัว
            cin.clear();                 // ล้างสถานะ error ของ cin
            cin.ignore(1000, '\n');      // ลบตัวอักษรที่เหลือใน buffer ทิ้ง
            cout << "Error: Please enter only ONE character\n"; // แจ้งข้อผิดพลาด
            continue;                   // วนกลับไปแสดงเมนูและรับ input ใหม่
        }

        if (menu == '4') {               // ถ้าผู้ใช้เลือก '4' หมายถึงต้องการออกจากโปรแกรม
            cout << "Goodbye!\n";        // แสดงข้อความลา
            break;                       // หยุด while loop ทำให้โปรแกรมไปถึง return 0
        } else if (menu == '3') {        // ถ้าผู้ใช้เลือก '3' หมายถึงต้องการดู ranking
            rank.showRank();             // เรียกฟังก์ชัน showRank() แสดงตาราง ranking บนหน้าจอ
        } else if (menu == '1' || menu == '2') { // ถ้าผู้ใช้เลือก '1' หรือ '2' หมายถึงต้องการเล่นเกม
            string playerName;           // ประกาศตัวแปร playerName สำหรับเก็บชื่อผู้เล่น
            cout << "Enter your name: "; // ขอให้ผู้เล่นใส่ชื่อ
            cin >> playerName;           // รับชื่อจากผู้ใช้เก็บใน playerName
            cin.ignore(1000, '\n');      // ล้าง newline ที่เหลือใน buffer หลังรับชื่อ

            Player p(playerName);        // สร้าง object p ชนิด Player โดยส่งชื่อที่รับมาให้ constructor

            if (menu == '2') {           // ถ้าเลือกเมนู '2' คือ hard mode
                HardBot hb("HardBot");   // สร้าง object hb ชนิด HardBot ตั้งชื่อว่า "HardBot"
                Game game(&p, &hb, true); // สร้าง object game โดยส่ง address ของ p และ hb และ true=hard mode
                cout << "[Hard Mode] Bot will counter your previous move!\n"; // แจ้งผู้เล่นว่าเป็น hard mode
                for (int i = 1; i <= 3; i++) { // วนลูปเล่นทั้งหมด 3 รอบ i=1,2,3
                    game.playRound(i);           // เรียก playRound ส่งเลขรอบปัจจุบัน
                    if (game.isExited()) break;  // ถ้า flag exited เป็น true (ผู้เล่นกด 0) ให้หยุด loop
                }
                if (!game.isExited()) game.showResult(rank); // ถ้าไม่ได้กด exit ให้แสดงผลสรุปและอัปเดต ranking
            } else {                     // ถ้าเลือกเมนู '1' คือ normal mode
                Bot b("Bot");            // สร้าง object b ชนิด Bot ตั้งชื่อว่า "Bot"
                Game game(&p, &b, false); // สร้าง object game โดยส่ง address ของ p และ b และ false=normal mode
                for (int i = 1; i <= 3; i++) { // วนลูปเล่นทั้งหมด 3 รอบ i=1,2,3
                    game.playRound(i);           // เรียก playRound ส่งเลขรอบปัจจุบัน
                    if (game.isExited()) break;  // ถ้า flag exited เป็น true ให้หยุด loop
                }
                if (!game.isExited()) game.showResult(rank); // ถ้าไม่ได้กด exit ให้แสดงผลสรุปและอัปเดต ranking
            }
        }
    }

    return 0;                // คืนค่า 0 ให้ระบบปฏิบัติการ หมายถึงโปรแกรมทำงานสำเร็จโดยไม่มีข้อผิดพลาด
}
