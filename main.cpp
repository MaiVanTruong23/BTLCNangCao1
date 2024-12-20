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
// Cac Ham Rang Buoc
// rang buoc so nguyen
//  Hàm nhập số nguyên với kiểm tra lỗi
int nhapSoNguyen(char *prompt)
{
    int KiemTra;
    char HamUserNhapVao[100];
    while (1)
    {
        printf("%s", prompt);
        fgets(HamUserNhapVao, sizeof(HamUserNhapVao), stdin);
        if (sscanf(HamUserNhapVao, "%d", &KiemTra) == 1)
        {
            return KiemTra;
        }
        else
        {
            printf("Nhap sai! Vui long nhap so nguyen.\n");
        }
    }
}
// Rang buoc ma tro khong giong nhau
//  Kiểm tra mã trọ có tồn tại trong cả danh sách đã lưu và trong danh sách hiện tại không
int kiemTraMaTroTonTai(Thue *headThue, Trong *headTrong, const char *maTro)
{
    Thue *currentThue = headThue;
    while (currentThue != NULL)
    {
        if (strcmp(currentThue->MaTro, maTro) == 0)
        {
            return 1; // Mã trọ đã tồn tại trong danh sách thuê
        }
        currentThue = currentThue->next;
    }
    Trong *currentTrong = headTrong;
    while (currentTrong != NULL)
    {
        if (strcmp(currentTrong->MaTro, maTro) == 0)
        {
            return 1; // Mã trọ đã tồn tại trong danh sách trống
        }
        currentTrong = currentTrong->next;
    }
    FILE *fileThue = fopen("Thue.txt", "r");
    if (fileThue != NULL)
    {
        char maTroTemp[15];
        while (fscanf(fileThue, "%[^,],", maTroTemp) == 1)
        {
            if (strcmp(maTroTemp, maTro) == 0)
            {
                fclose(fileThue);
                return 1;
            }
            fscanf(fileThue, "%*[^,],%*[^,],%*[^,],%*[^,],%*d,%*d,%*d,%*d,%*d,%*s\n");
        }
        fclose(fileThue);
    }
    FILE *fileTrong = fopen("Trong.txt", "r");
    if (fileTrong != NULL)
    {
        char maTroTemp[15];
        while (fscanf(fileTrong, "%[^,],", maTroTemp) == 1)
        {
            if (strcmp(maTroTemp, maTro) == 0)
            {
                fclose(fileTrong);
                return 1;
            }
            fscanf(fileTrong, "%*[^,],%*[^,],%*[^,],%*[^,],%*d,%*d,%*d,%*d,%*s\n");
        }
        fclose(fileTrong);
    }

    return 0;
}

// Ham logic
Thue *docDanhSachThue()
{
    FILE *file = fopen("Thue.txt", "r");
    if (file == NULL)
    {
        printf("Khong co file nao de doc.\n");
        return NULL;
    }

    Thue *danhSach1 = NULL;
    while (!feof(file))
    {
        Thue *p = (Thue *)malloc(sizeof(Thue));
        if (p == NULL)
        {
            printf("Khong the cap phat bo nho.\n");
            fclose(file);
            return NULL;
        }
        if (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%d,%d,%[^\n]\n",
                   p->MaTro, p->HoTenKT, p->Diachitro, p->SDTKT,
                   &p->Dien, &p->Nuoc, &p->DichVu, &p->TienPhong,
                   &p->TongTien, p->TrangThai) == 10)
        {
            p->next = danhSach1;
            danhSach1 = p;
        }
        else
        {
            free(p);
        }
    }

    fclose(file);
    return danhSach1;
}

Trong *docDanhSachTrong()
{
    FILE *file = fopen("Trong.txt", "r");
    if (file == NULL)
    {
        printf("Khong co file nao de doc.\n");
        return NULL;
    }

    Trong *danhSach = NULL;
    while (!feof(file))
    {
        Trong *v = (Trong *)malloc(sizeof(Trong));
        if (v == NULL)
        {
            printf("Khong the cap phat bo nho.\n");
            fclose(file);
            return NULL;
        }
        if (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%d,%[^\n]\n",
                   v->MaTro, v->HoTenCT, v->Diachitro, v->SDTCT,
                   &v->DienSo, &v->NuocSo, &v->DichVuThang, &v->TienPhong, v->TrangThai) == 9)
        {
            // Thêm bản ghi vào danh sách
            v->next = danhSach;
            danhSach = v;
        }
        else
        {
            free(v);
        }
    }

    fclose(file);
    return danhSach;
}
