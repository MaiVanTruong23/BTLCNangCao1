#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

// Cac Ham Phu Giup trinh bay dep hon:>
void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = {x, y};
    SetConsoleCursorPosition(h, c);
}
void maximizeConsole()
{
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE);
}

void hieuung(const char *text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        putchar(text[i]);
        fflush(stdout);
        Sleep(30);
    }
    putchar('\n');
}
// Cac ham khai bao cac struct
typedef struct TKMK
{
    char tk[50];
    char mk[50];
    int role;
    struct TKMK *next;
} TKMK;

typedef struct Thue
{
    char MaTro[15];
    char HoTenKT[30];
    char Diachitro[100];
    char SDTKT[20];
    int Dien;
    int Nuoc;
    int DichVu;
    int TienPhong;
    int TongTien;
    char TrangThai[20];
    struct Thue *next;
} Thue;

typedef struct Trong
{
    char MaTro[15];
    char HoTenCT[30];
    char Diachitro[100];
    char SDTCT[20];
    int DienSo;
    int NuocSo;
    int DichVuThang;
    int TienPhong;
    char TrangThai[20];
    struct Trong *next;
} Trong;

typedef struct
{
    char MaTro[15];
    char HoTenKT[30];
    char SDTKT[20];
    char PhanHoi[200];
} PhanHoi;
typedef struct
{
    char NoiDung[200];
    char Ngay[20];
} ThongBao;