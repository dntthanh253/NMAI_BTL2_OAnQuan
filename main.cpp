#include <graphics.h>
#include <conio.h>
#include <time.h>

#define MAXX 900 // Độ rộng cửa sổ
#define MAXY 600 // Độ cao cửa sổ
#define mauNen 0xdfdfff // Nền
#define mauVien 0x7d77f8 // Màu viền 
#define mauBanCo 0xdfffff // Nền bàn cờ
#define mauQuanCo 169 // Màu quân cờ
#define mauMuiTen 12 // Màu mũi tên
#define mauMenu 0xffffdf // Màu menu
#define mauVienMenu 0xffffff // Màu menu
#define Y 300 // Tọa độ Y của bàn cờ

struct Point {
    int x;
    int y;
};

int oHienTai = 0, DaiDan = 0;  // Ô hiện tại, Số dân hiện tại?
char c; // Biến nhận kí hiệu nhập từ bàn phím
int Player; // Người chơi
int Selected = 0; // Đã chọn
int Game = 1; // Trạng thái game
int Diem[2] = {0, 0}; // Điểm của 2 người chơi
int Computer, Diff; // Chế độ chơi (=0:2 người; =1: chơi với máy), Độ khó?

// Các chuỗi thông báo
char TextPlayer[2][9]; 
char TextPvP[2][9] = {"Player 2", "Player 1"};
char TextPvC[2][9] = {"Computer", "Player"};
char TxtSpace[] = "              ";
char TxtEasy[] = "Easy";
char TxtNormal[] = "Normal";
char TxtOAnQuan[] = "O an quan";
char TxtBTL[] = "Nhap mon AI - BTL2 - 2114749 - 2110934";

// Tính trị tuyệt đối
int triTuyetDoi(int x) {
    return x > 0 ? x : -x;
}

// Vẽ nền
void veKhungNen() {
    setcolor(mauBanCo); // Màu viền 
    setfillstyle(1, mauNen); // Màu nền
    int doDayVien = 10; // Độ dày viền
    for (int i = 0; i <= doDayVien; i++){
        rectangle(i, i, MAXX - i, MAXY - i); // Vẽ viền Left -> Top -> Right -> Bottom
    }
    bar(doDayVien, doDayVien, MAXX - doDayVien, MAXY - doDayVien); // Vẽ hình chữ nhật = nền
}

// Tọa độ các dân ứng theo số lượng dân
Point toaDoDan[] = {{0, 0}, //bỏ qua số lượng 0
                {0, 0},
                {-25, -25},
                {25, -25},
                {-25, 25},
                {25, 25},
                {0, -25},
                {-25, 0},
                {25, 0},
                {0, 25},
                {-12, -12},
                {12, -12},
                {-12, 12},
                {12, 12},
                {-12, -25},
                {12, -25},
                {-12, 25},
                {12, 25},
                {-25, -12},
                {25, -12},
                {-25, 12},
                {25, 12}}; //ước chừng tối đa là 21 dân/1 ô dân 
               
//Tọa độ dân trong ô quan
Point toaDoDanQuan[] = {{0, 0}, //bỏ qua số lượng 0
                {35, -75},
                {35, -60},
                {35, -45},
                {35, -30},
                {35, -15},
                {35, 0},
                {35, 15},
                {35, 30},
                {35, 45},
                {35, 60},
                {35, 75},
                {20, -75},
                {20, -60},
                {20, -45},
                {20, -30},
                {20, -15},
                {20, 0},
                {20, 15},
                {20, 30},
                {20, 45},
                {20, 60},
                {20, 75}};

class OAQ {
  private:
  public:
    int conQuan; // Còn quan
    int soDan; // Số dân
    void veDan(); // Vẽ dân
} oaq[12]; // Mảng 12 ô, 0 & 6: quan, 1-5 & 7-11: dân

// Vẽ dân
void OAQ::veDan() {
    int tamO; // Vị trí tâm của ô trên trục X
    int tdX, tdY; // Tọa độ dân

    // Nếu là ô quan (0 hoặc 6)
    if (oHienTai % 6 == 0) {
        tamO = 150 + (oHienTai) * 100; //n
        tdX = tamO + (1 - oHienTai /3) * toaDoDanQuan[this->soDan].x; 
        tdY = Y + toaDoDanQuan[this->soDan].y;
    } 
    // Nếu là ô dân 
    else{
        tamO = 50 + (7 - triTuyetDoi(6 - oHienTai)) * 100;
        tdX = tamO + toaDoDan[this->soDan].x;
        tdY = Y - 50 + (oHienTai/6) * 100 + toaDoDan[this->soDan].y;
    }

    // Vẽ dân
    setcolor(mauQuanCo); 
    setfillstyle(1, mauQuanCo);
    circle(tdX, tdY, 4);
    floodfill(tdX, tdY, mauQuanCo);
}

// Vẽ dân khi ván đấu bắt đầu, không có dân trong ô quan
void veDan(int soO, int soDan) {
    // Xác định vị trí của dân
    int tamO = (7 - triTuyetDoi(6 - soO)) * 100 + 50; 
    int tdX = tamO + toaDoDan[soDan].x;
    int tdY = Y + 50 * ((soO / 6) * 2 - 1) + toaDoDan[soDan].y;
    
    setcolor(mauQuanCo);
    setfillstyle(1, mauQuanCo);
    circle(tdX, tdY, 4);
    floodfill(tdX, tdY, mauQuanCo);
}

// Vẽ quan
void veQuan(int soO) {
    int tamO = 150 + soO * 100;
    setcolor(mauQuanCo);
    setfillstyle(1, mauQuanCo);
    ellipse(tamO - 20 * (1 - soO /3), Y, 0, 360, 10, 20);
    floodfill(tamO - 20 * (1 - soO /3), Y, mauQuanCo);
}

// Vẽ bàn cờ 
void veBanCo() {
    // Khung bàn cờ
    setcolor(mauVien); // Màu đường
    line(200, Y - 100, 700, Y - 100); // Vẽ đường ngang trên chiều dài 500
    line(200, Y + 100, 700, Y + 100); // Vẽ đường ngang dưới
    arc(200, Y, 90, 270, 100); // Vẽ cung trái
    arc(700, Y, -90, 90, 100); // Vẽ cung phải
    
    setfillstyle(1, mauBanCo); 
    floodfill(400, Y, mauVien);

    for (int i = 0; i <= 5; i++)
        line(200 + 100 * i, Y - 100, 200 + 100 * i, Y + 100);
    line(200, Y, 700, Y);

    // Vẽ quan và dân
    for (int i = 0; i < 12; i++) {
        if (i % 6) {
            for (int j = 1; j <= 5; j++)
            veDan(i, j);
        } else veQuan(i);
    }
}

//Xóa quan bằng cách vẽ đè lên quan với màu nền
void XoaQuan() {
    setcolor(mauBanCo);
    setfillstyle(1, mauBanCo);
    int tamO = 150 + oHienTai * 100;
    // pieslice(tamO - 20 * (1 - oHienTai /3), Y, 0, 360, 10);
    // floodfill(tamO - 20 * (1 - oHienTai /3), Y, mauBanCo);
    if (oHienTai == 0)
        pieslice(tamO + 45, Y, 90, 270, 85);
    else
        pieslice(tamO - 45, Y, -90, 90, 85);
}

void XoaO() {
    int tamO = (7 - triTuyetDoi(6 - oHienTai)) * 100 + 50;
    int tdY = Y + 50 * ((oHienTai / 6) * 2 - 1);  
    setfillstyle(1, mauBanCo);
    bar(tamO - 48, tdY - 48, tamO + 48, tdY + 48);
}

// Vẽ mũi tên
void VeMuiTen() {
    // Tọa độ mũi tên
    int tdX = (7 - triTuyetDoi(6 - oHienTai)) * 100 + 50;
    int doLech = (oHienTai / 6)*2 - 1; // độ lệch theo trục Y
    int tdY = Y + doLech * 110;
    setcolor(mauMuiTen);
    // Vẽ mũi tên
    line(tdX, tdY, tdX, tdY + 40 * doLech);
    line(tdX, tdY, tdX + 12 * doLech, tdY + 12 * doLech);
    line(tdX, tdY, tdX - 12 * doLech, tdY + 12 * doLech);
}

// Xóa mũi tên bằng cách vẽ đè lên mũi tên với màu nền
void XoaMuiTen() {
    // Tọa độ mũi tên
    int tdX = (7 - triTuyetDoi(6 - oHienTai)) * 100 + 50;
    int doLech = (oHienTai / 6)*2 - 1; // độ lệch theo trục Y
    int tdY = Y + doLech * 110;
    
    // Xóa mũi tên
    setcolor(mauNen);
    setlinestyle(0, 0, 4);
    line(tdX, tdY, tdX, tdY + 40 * doLech);
    line(tdX, tdY, tdX + 12 * doLech, tdY + 12 * doLech);
    line(tdX, tdY, tdX - 12 * doLech, tdY + 12 * doLech);
}

// Xác định ô được chọn
void ChonO() {
    int tamOx = (7 - triTuyetDoi(6 - oHienTai)) * 100 + 50;
    int tamOy = Y + 50 * ((oHienTai / 6) * 2 - 1);
    setcolor(0x5f00af);
    line(tamOx - 44, tamOy - 44, tamOx - 25, tamOy - 44);
    line(tamOx - 44, tamOy - 44, tamOx - 44, tamOy - 25);
    line(tamOx + 44, tamOy + 44, tamOx + 25, tamOy + 44);
    line(tamOx + 44, tamOy + 44, tamOx + 44, tamOy + 25);
}

// Bỏ chọn ô bằng cách vẽ đè lên ô với màu nền
void BoChonO() {
    int tamOx = (7 - triTuyetDoi(6 - oHienTai)) * 100 + 50;
    int tamOy = Y + 50 * ((oHienTai / 6) * 2 - 1);
    setcolor(mauBanCo);
    setlinestyle(0, 0, 4); 
    line(tamOx - 44, tamOy - 44, tamOx - 25, tamOy - 44);
    line(tamOx - 44, tamOy - 44, tamOx - 44, tamOy - 25);
    line(tamOx + 44, tamOy + 44, tamOx + 25, tamOy + 44);
    line(tamOx + 44, tamOy + 44, tamOx + 44, tamOy + 25);

}

// Cập nhật điểm
void capNhatDiem() {
    char *text = new char[14];
    sprintf(text, "%s: %d ", TextPlayer[Player], Diem[Player]);
    setcolor(0);
    outtextxy(100  + Player * 450, 470, text);
    delete[] text;
}

// Cập nhật số dân còn trong tay khi rải quân
void capNhatKhiRai(int soDan) {
    char *text = new char[13];
    sprintf(text, "Con lai: %d ", soDan);
    setcolor(1);
    outtextxy(100 + Player * 450, 520, text);
    delete[] text;
}

// Xóa hiển thị "Còn lại" khi kết thúc lượt
void xoaConLai() {
    outtextxy(100 + Player * 450, 520, TxtSpace);
}

// Kết thúc ván đấu
void ketThucGame() {
    char *text = new char[20];
    setcolor(0);
    sprintf(text, "%s: %d", TextPlayer[1], Diem[1]);
    outtextxy(100, 170, text);
    sprintf(text, "%s: %d", TextPlayer[0], Diem[0]);
    outtextxy(100, 250, text);
    sprintf(text, "Game winner: %s", TextPlayer[Diem[0] > Diem[1] ? 0 : 1]);
    outtextxy(100, 330, text);
    delete[] text;
}

int PlayerTurn() {
    int huongRai; // Hướng rải quân
    while (Selected != 2) {
        c = getch(); // Nhận kí hiệu từ bàn phím
        switch (c) {
        case 75: // Mũi tên trái
            // Nếu chưa chọn ô
            if (Selected == 0) {
                XoaMuiTen();
                do {
                    oHienTai -= (1 - 2 * Player); //Người chơi phía trên: 0, người chơi phía dưới: 1
                    if (oHienTai == Player * 12)  
                        oHienTai = Player * 2 + 5; // Nếu ô hiện tại là ô quan của người chơi, quay lại ô 5 hoặc 7
                } while (oaq[oHienTai].soDan == 0); // Nếu ô hiện tại không có dân, chuyển sang ô tiếp theo
                VeMuiTen();
            } 
            // Nếu đã chọn ô, phím mũi tên trái sẽ chuyển sang chế độ rải quân sang trái
            else {
                Selected = 2;
                huongRai = Player * 2 - 1; //Người chơi dưới đi theo hướng ô tăng index và ngược lại
            }
            break;
        case 77: // Mũi tên phải tương tự như mũi tên trái
            if (Selected == 0) {
                XoaMuiTen();
                do {
                    oHienTai += (1 - 2 * Player);
                    if (oHienTai == 6)
                        oHienTai = Player * 10 + 1;
                } while (oaq[oHienTai].soDan == 0);
                VeMuiTen();
            } else {
                Selected = 2;
                huongRai = 1 - Player * 2; //Người chơi dưới đi theo hướng ô giảm index và ngược lại
            }
            break;
        case 13: // Enter, nếu ô chưa được chọn thì chọn ô , nếu đã chọn thì bỏ chọn
            if (Selected == 0) {
                capNhatKhiRai(oaq[oHienTai].soDan);
                ChonO();
                Selected = 1; 
            } else {
                xoaConLai();
                BoChonO();
                Selected = 0;
            }
            break;
        }
    }
    return huongRai; // Trả về hướng di chuyển
}

// Tính toán điểm - AI
int Calculate(int oComputer, int huongRai) {
    int diemTamTinh = 0;
    OAQ theCo[12];
    for (int i = 0; i < 12; i++) {
        theCo[i].soDan = oaq[i].soDan;
        if (i % 6 == 0)
            theCo[i].conQuan = oaq[i].conQuan;
    }
    int soDanNhat = theCo[oComputer].soDan;
    theCo[oComputer].soDan = 0;
    while (true) {
        if (soDanNhat == 0) {
            oComputer += huongRai;
            if (oComputer == 12)
                oComputer = 0;
            else if (oComputer == -1)
                oComputer = 11;
            if (oComputer % 6) {
                soDanNhat = theCo[oComputer].soDan;
                if (soDanNhat == 0) {
                    do {
                        oComputer += huongRai;
                        if (oComputer == 12)
                            oComputer = 0;
                        else if (oComputer == -1)
                            oComputer = 11;
                        if (oComputer % 6 == 0) {
                            if (theCo[oComputer].conQuan) {
                                if (theCo[oComputer].soDan >= 5) {
                                    diemTamTinh += (theCo[oComputer].soDan + 10);
                                    theCo[oComputer].soDan = 0;
                                    theCo[oComputer].conQuan = 0;
                                } else
                                    break;
                            } else if (theCo[oComputer].soDan > 0) {
                                diemTamTinh += theCo[oComputer].soDan;
                                theCo[oComputer].soDan = 0;
                            } else
                                break;

                        } 
                        //nếu là ô dân
                        else if (theCo[oComputer].soDan > 0) {
                            diemTamTinh += theCo[oComputer].soDan;
                            theCo[oComputer].soDan = 0;
                        } else
                            break;
                            
                        oComputer += huongRai;
                        if (oComputer == 12)
                            oComputer = 0;
                        else if (oComputer == -1)
                            oComputer = 11;
                        soDanNhat = theCo[oComputer].soDan;
                    } while (soDanNhat == 0);
                    break;
                }
                theCo[oComputer].soDan = 0;
            } else
                break;
        }
        soDanNhat--;
        oComputer += huongRai;
        if (oComputer == 12)
            oComputer = 0;
        else if (oComputer == -1)
            oComputer = 11;
        theCo[oComputer].soDan++;
    }
    return diemTamTinh;
}

//Tìm ô & hướng rải quân tối ưu
int CalculateMax(int &huongRai) {
    //Tính tại ô hiện tại
        int max = Calculate(oHienTai, 1); 
        
        int Otmp = oHienTai; 
        int Current = oHienTai + 1;

        huongRai = 1; //Tạm gán hướng rải tối ưu là đi theo hướng index tăng 
        //Tính điểm tạm thời nếu đi theo hướng index giảm và so sánh với điểm tối ưu
        int k = Calculate(oHienTai, -1); 
        if (k > max) {
            max = k;
            huongRai = -1;
        }


    
    //Chuyển sang ô tiếp theo để xét điểm tối ưu
        while (Current < 6) {
            //Nếu ô hiện tại còn dân
            if (oaq[Current].soDan > 0) {
                //Tính điểm theo 2 hướng và so sánh với điểm tối ưu
                k = Calculate(Current, 1);
                if (k > max) {
                    Otmp = Current;
                    max = k;
                    huongRai = 1;
                }
                k = Calculate(Current, -1);
                if (k > max) {
                    Otmp = Current;
                    max = k;
                    huongRai = -1;
                }
            }
            //Chuyển sang ô tiếp theo nêu ô hiện tại trống
            Current++;
        }
    return Otmp; //Trả về ô tối ưu
}

int ComputerTurn() {
    int oMayChon, huongRai;
    //Nếu độ khó là dễ (1) -> máy rải ngẫu nhiên (tệ)
    if (Diff == 1) {
        int n = rand() % 5; //Số lần chọn ô ngẫu nhiên
        huongRai = rand() % 2 * 2 - 1; //Hướng rải ngẫu nhiên
        for (int i = 0; i < n; i++) {
            XoaMuiTen();
            delay(400);
            //Chỉ chọn các ô còn dân
            do {
                oHienTai++;
                if (oHienTai == 6)
                    oHienTai = 1;
            } while (oaq[oHienTai].soDan == 0);
            VeMuiTen();
            delay(400); //Để có thể nhìn thấy ô được chọn
        }
    } else{ //Nếu độ khó là khó -> máy rải dựa vào điểm tối ưu (độ sâu = 1)
        oMayChon = CalculateMax(huongRai);
        delay(400);
        int i = 0;
        while (oHienTai != oMayChon) {
            XoaMuiTen();
            do {
                oHienTai++;
            } while (oaq[oHienTai].soDan == 0);
            VeMuiTen();
            delay(400); //Để có thể nhìn thấy ô được chọn
        }
    }
  
    capNhatKhiRai(oaq[oHienTai].soDan);
    ChonO();
    delay(400);
    //Khi kết thúc, oHienTai sẽ là ô cuối cùng được chọn
    return huongRai;
}

int PvP() {
    return PlayerTurn();
}

int PvC() { 
    if (Player == 0)
        return ComputerTurn();
    return PlayerTurn();
}

//Trỏ mũi tên vào chế độ chơi
void MenuArrow(int chonDoKho) {
    chonDoKho *= 80;
    line(180, 280 + chonDoKho, 200, 280 + chonDoKho);
    line(180, 290 + chonDoKho, 200, 290 + chonDoKho);
    line(193, 273 + chonDoKho, 205, 285 + chonDoKho);
    line(193, 297 + chonDoKho, 205, 285 + chonDoKho);
}

//Khi chọn đấu với máy
int Difficult() {
    int chonDoKho = 0, a = 0; //Tạm thời 0 là dễ
    setbkcolor(mauMenu);
    setfillstyle(1, mauMenu);
    setcolor(mauVienMenu);
    rectangle(500, 250, 700 , 350);
    bar(501, 251, 699, 349);

    setcolor(12);
    outtextxy(550, 260, TxtEasy);
    outtextxy(550, 310, TxtNormal);
    line(510, 275, 530, 275);
    line(520, 267, 530, 275);
    line(520, 283, 530, 275);
    
    while (true) {
        c = getch(); //Nhận kí hiệu từ bàn phím
        switch (c) {
        case 72: // Mũi tên lên
        case 80: // Mũi tên xuống
            //Xóa mũi tên cũ
            setcolor(mauMenu);
            setlinestyle(0, 0, 4);
            line(510, 275 + a, 530, 275 + a);
            line(520, 267 + a, 530, 275 + a);
            line(520, 283 + a, 530, 275 + a);
            chonDoKho = 1 - chonDoKho; //Nếu đang 0 -> 1, nếu đang 1 -> 0
            a = chonDoKho * 48;
            setcolor(12);
            line(510, 275 + a, 530, 275 + a);
            line(520, 267 + a, 530, 275 + a);
            line(520, 283 + a, 530, 275 + a);
            break;
        case 13: // Enter
            return chonDoKho + 1; //Trả về độ khó 1 (Dễ), 2 (Khó)
        }
    }
}

void ClearDiff() { 
    setcolor(mauNen);
    setfillstyle(1, mauNen);
    bar(30, 300, 870, 570);
}

void MenuGame() {
    veKhungNen();
    char TextMenu[][9] = {"1 Player", "2 Player"};
    int chonCheDo = 0; 
    char c;
    setbkcolor(mauNen);

    //Tiêu đề
    settextstyle(10, 0, 9); 
    setcolor(0x875f00); 
    outtextxy(200, 30, TxtOAnQuan);
    
    //BTL
    settextstyle(10, 0, 1); 
    setcolor(0x00005f);
    outtextxy(350, 120, TxtBTL);

    //Menu
    setcolor(mauVienMenu);
    setlinestyle(0, 0, 2);
    rectangle(220, 250, 420, 320); 
    rectangle(200, 330, 400, 400);
    settextstyle(10, 0, 4);
    setcolor(0x5f00af);
    MenuArrow(0);
    outtextxy(246, 270, TextMenu[0]);
    setcolor(12);
    outtextxy(226, 350, TextMenu[1]);
    while (true) {
        c = getch();
        switch (c) {
        case 72:
        case 75:
        case 77:
        case 80:
            //Xóa option cũ
            setcolor(mauNen);
            rectangle(220, 250 + chonCheDo * 80, 420, 320 + chonCheDo * 80);
            rectangle(200, 330 - chonCheDo * 80, 400, 400 - chonCheDo * 80);
            outtextxy(246, 270 + chonCheDo * 80, TextMenu[chonCheDo]);
            outtextxy(226, 350 - chonCheDo * 80, TextMenu[1 - chonCheDo]);
            MenuArrow(chonCheDo);

            //Chọn option mới
            setcolor(mauVienMenu);
            chonCheDo = 1 - chonCheDo; //Nếu đang 0 -> 1, nếu đang 1 -> 0 
            rectangle(220, 250 + chonCheDo * 80, 420, 320 + chonCheDo * 80);
            rectangle(200, 330 - chonCheDo * 80, 400, 400 - chonCheDo * 80);
            setcolor(0x5f00af);
            MenuArrow(chonCheDo);
            outtextxy(246, 270 + chonCheDo * 80, TextMenu[chonCheDo]);
            setcolor(12);
            outtextxy(226, 350 - chonCheDo * 80, TextMenu[1 - chonCheDo]);
            break;

        case 13: // Enter
            if (chonCheDo == 1) { //   2 người chơi
                Computer = 0;
                return;
            }
            Diff = Difficult();
            if (Diff) {
                Computer = 1;
                return;
            }
            ClearDiff();
            setbkcolor(mauNen);
            break;
        }
    }
}

int main() {
    srand(time(NULL)); // Tạo số ngẫu nhiên
    initwindow(MAXX , MAXY , "Nhap mon AI - BTL2 - 2114749 - 2110934"); // Khởi tạo cửa sổ đồ họa
    veKhungNen();
    MenuGame();
    if (Computer == 0) {// 2 người chơi
        strcpy(TextPlayer[0], TextPvP[0]); // Sao chép chuỗi
        strcpy(TextPlayer[1], TextPvP[1]);
    } else {
        strcpy(TextPlayer[0], TextPvC[0]); // Sao chép chuỗi
        strcpy(TextPlayer[1], TextPvC[1]);
    }
    int (*P[2])(); // Con trỏ hàm
    P[0] = &PvP;
    P[1] = &PvC;
    veKhungNen();
    setbkcolor(mauNen);
    settextstyle(10, 0, 3);

    //Khởi tạo giá trị bàn cờ
    int huongRai, diemTamTinh;
    for (int i = 0; i < 12; i++) {
        if (i % 6 == 0) {
            oaq[i].soDan = 0;
            oaq[i].conQuan = 1;
        } else
            oaq[i].soDan = 5;
    }

    veBanCo();

    //Hiển thị điểm
    setcolor(0);
    Player = 0;
    capNhatDiem();
    Player = 1;
    capNhatDiem();

// Hỏi người chơi xem chọn đi trước hoặc sau
    setcolor(0);
    outtextxy(100, 100, "Ban muon di truoc hay sau? (Enter: Truoc, Space: Sau) ");
    while (true) {
        c = getch();
        if (c == 13)// Enter
        {
            Player = 1;
            oHienTai = 7;
            setcolor(mauNen);
            outtextxy(100, 100, "Ban muon di truoc hay sau? (Enter: Truoc, Space: Sau) ");
            //Tiêu đề
            settextstyle(10, 0, 9); 
            setcolor(0x875f00); 
            outtextxy(200, 30, TxtOAnQuan);
            
            //BTL
            settextstyle(10, 0, 1); 
            setcolor(0x00005f);
            outtextxy(350, 120, TxtBTL);
            break;
        } else if (c == 32) {
            Player = 0;
            oHienTai = 1;
            break;
            setcolor(mauNen);
            outtextxy(100, 100, "Ban muon di truoc hay sau? (Enter: Truoc, Space: Sau) ");
            //Tiêu đề
            settextstyle(10, 0, 9); 
            setcolor(0x875f00); 
            outtextxy(200, 30, TxtOAnQuan);
            
            //BTL
            settextstyle(10, 0, 1); 
            setcolor(0x00005f);
            outtextxy(350, 120, TxtBTL);
        }
    }

    while (true) {
        VeMuiTen();
        huongRai = P[Computer]();  //Computer=1 -> PvC, gọi lượt chơi của người chơi
        Selected = 0;
        XoaMuiTen();
        //Nhặt dân
        DaiDan = oaq[oHienTai].soDan;
        oaq[oHienTai].soDan = 0;
        XoaO();
        while (true) {
            delay(400);
            // Nếu ô hiện tại không có dân, chuyển sang ô tiếp theo
            if (DaiDan == 0) {
                oHienTai += huongRai;
                if (oHienTai == 12)
                    oHienTai = 0;
                else if (oHienTai == -1)
                    oHienTai = 11;
                if (oHienTai % 6) {
                    XoaO();
                    DaiDan = oaq[oHienTai].soDan;
                    capNhatKhiRai(DaiDan);
                    if (DaiDan == 0) {
                        do {
                            oHienTai += huongRai;
                            if (oHienTai == 12)
                                oHienTai = 0;
                            else if (oHienTai == -1)
                                oHienTai = 11;
                            if (oHienTai % 6 == 0) {
                                if (oaq[oHienTai].conQuan) {
                                    if (oaq[oHienTai].soDan >= 5) {
                                        Diem[Player] += (oaq[oHienTai].soDan + 10);
                                        capNhatDiem();
                                        oaq[oHienTai].soDan = 0;
                                        oaq[oHienTai].conQuan = 0;
                                        XoaQuan();
                                        if (oaq[6 - oHienTai].soDan == 0 && oaq[6 - oHienTai].conQuan == 0)
                                            Game = 0;
                                    } else
                                        break;
                                } else if (oaq[oHienTai].soDan > 0) {
                                    Diem[Player] += oaq[oHienTai].soDan;
                                    capNhatDiem();
                                    oaq[oHienTai].soDan = 0;
                                    XoaQuan();
                                    if (oaq[6 - oHienTai].soDan == 0 && oaq[6 - oHienTai].conQuan == 0)
                                        Game = 0;
                                } else
                                    break;
                            } else if (oaq[oHienTai].soDan > 0) {
                                Diem[Player] += oaq[oHienTai].soDan;
                                capNhatDiem();
                                oaq[oHienTai].soDan = 0;
                                XoaO();
                            } else
                                break;
                            oHienTai += huongRai;
                            if (oHienTai == 12)
                                oHienTai = 0;
                            else if (oHienTai == -1)
                                oHienTai = 11;
                            DaiDan = oaq[oHienTai].soDan;
                            delay(400);
                        } while (DaiDan == 0);
                        break;
                    }
                    oaq[oHienTai].soDan = 0;
                    delay(400);
                } else
                    break;
            }
            DaiDan--;
            capNhatKhiRai(DaiDan);
            oHienTai += huongRai;
            if (oHienTai == 12)
                oHienTai = 0;
            else if (oHienTai == -1)
                oHienTai = 11;
            oaq[oHienTai].soDan++;
            settextstyle(10, 0, 3);
            oaq[oHienTai].veDan();
        }
        if (!Game)
            break;
        xoaConLai();
        Player = 1 - Player;
        diemTamTinh = 1;
        do {
            oHienTai = 6 * Player + diemTamTinh;
            diemTamTinh++;
        } while (oaq[oHienTai].soDan == 0 && diemTamTinh <= 5);
        if (oaq[oHienTai].soDan == 0) {
            Diem[Player] -= 5;
            capNhatDiem();
            setcolor(8);
            for (int i = 1 + 6 * Player; i < 6 + 6 * Player; i++) {
                veDan(i, 1);
                oaq[i].soDan = 1;
            }
            oHienTai = 6 * Player + 1;
        }
    }
    //Tính điểm những quân trên bàn còn lại khi kết thúc game
    for (int i = 1; i < 6; i++)
        Diem[0] += oaq[i].soDan;
    for (int i = 7; i < 12; i++)
        Diem[1] += oaq[i].soDan;

    cleardevice();
    veKhungNen();
    setcolor(0);
    ketThucGame();
    getch();
    closegraph(-2);
    return 0;
}
