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
// Ham tao danh sach lien ket
Thue *themThue(Thue *head, Thue data)
{
    Thue *newNode = (Thue *)malloc(sizeof(Thue));
    if (newNode == NULL)
    {
        printf("Khong the mo file!");
        return head;
    }

    *newNode = data;
    newNode->next = NULL;

    if (head == NULL)
    {
        return newNode;
    }

    Thue *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return head;
}
Trong *themTrong(Trong *head, Trong data)
{
    Trong *newNode = (Trong *)malloc(sizeof(Trong));
    if (newNode == NULL)
    {
        printf("Khong the mo file!");
        return head;
    }

    *newNode = data;
    newNode->next = NULL;

    if (head == NULL)
    {
        return newNode;
    }

    Trong *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

// Cac Ham ghi va luu vo file
void ghiDanhSachThueVaoFile(Thue *head)
{
    FILE *tempFile = fopen("Thue_temp.txt", "w"); // Mở file tạm để ghi
    if (tempFile == NULL)
    {
        printf("Khong the mo file tam de ghi!\n");
        return;
    }

    Thue *current = head;
    while (current != NULL)
    {
        fprintf(tempFile, "%s,%s,%s,%s,%d,%d,%d,%d,%d,%s\n",
                current->MaTro,
                current->HoTenKT,
                current->Diachitro,
                current->SDTKT,
                current->Dien,
                current->Nuoc,
                current->DichVu,
                current->TienPhong,
                current->TongTien, // Tổng tiền được ghi
                current->TrangThai);
        current = current->next;
    }

    fclose(tempFile);

    // Thay thế file cũ bằng file tạm
    if (remove("Thue.txt") == 0)
    { // Xóa file gốc
        if (rename("Thue_temp.txt", "Thue.txt") == 0)
        { // Đổi tên file tạm thành file gốc
            printf("Ghi danh sach thue thanh cong vao file Thue.txt.\n");
        }
        else
        {
            printf("Khong the doi ten file tam thanh file Thue.txt!\n");
        }
    }
    else
    {
        printf("Khong the xoa file Thue.txt cu!\n");
    }
}

void ghiDanhSachTrongVaoFile(Trong *head)
{
    FILE *tempFile = fopen("Trong_temp.txt", "w"); // Mở file tạm
    if (tempFile == NULL)
    {
        printf("Khong the mo file tam de ghi!\n");
        return;
    }

    Trong *current = head;
    while (current != NULL)
    {
        fprintf(tempFile, "%s,%s,%s,%s,%d,%d,%d,%d,%s\n",
                current->MaTro,
                current->HoTenCT,
                current->Diachitro,
                current->SDTCT,
                current->DienSo,
                current->NuocSo,
                current->DichVuThang,
                current->TienPhong,
                current->TrangThai);
        current = current->next;
    }

    fclose(tempFile);

    // Thay thế file gốc bằng file tạm
    remove("Trong.txt");                   // Xóa file gốc
    rename("Trong_temp.txt", "Trong.txt"); // Đổi tên file tạm thành file gốc
    printf("Ghi danh sach trong thanh cong vao file Trong.txt.\n");
}
// Ham tao danh sach lien ket
Thue *themThue(Thue *head, Thue data)
{
    Thue *newNode = (Thue *)malloc(sizeof(Thue));
    if (newNode == NULL)
    {
        printf("Khong the mo file!");
        return head;
    }

    *newNode = data;
    newNode->next = NULL;

    if (head == NULL)
    {
        return newNode;
    }

    Thue *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return head;
}
Trong *themTrong(Trong *head, Trong data)
{
    Trong *newNode = (Trong *)malloc(sizeof(Trong));
    if (newNode == NULL)
    {
        printf("Khong the mo file!");
        return head;
    }

    *newNode = data;
    newNode->next = NULL;

    if (head == NULL)
    {
        return newNode;
    }

    Trong *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

// Cac Ham ghi va luu vo file
void ghiDanhSachThueVaoFile(Thue *head)
{
    FILE *tempFile = fopen("Thue_temp.txt", "w"); // Mở file tạm để ghi
    if (tempFile == NULL)
    {
        printf("Khong the mo file tam de ghi!\n");
        return;
    }

    Thue *current = head;
    while (current != NULL)
    {
        fprintf(tempFile, "%s,%s,%s,%s,%d,%d,%d,%d,%d,%s\n",
                current->MaTro,
                current->HoTenKT,
                current->Diachitro,
                current->SDTKT,
                current->Dien,
                current->Nuoc,
                current->DichVu,
                current->TienPhong,
                current->TongTien, // Tổng tiền được ghi
                current->TrangThai);
        current = current->next;
    }

    fclose(tempFile);

    // Thay thế file cũ bằng file tạm
    if (remove("Thue.txt") == 0)
    { // Xóa file gốc
        if (rename("Thue_temp.txt", "Thue.txt") == 0)
        { // Đổi tên file tạm thành file gốc
            printf("Ghi danh sach thue thanh cong vao file Thue.txt.\n");
        }
        else
        {
            printf("Khong the doi ten file tam thanh file Thue.txt!\n");
        }
    }
    else
    {
        printf("Khong the xoa file Thue.txt cu!\n");
    }
}

void ghiDanhSachTrongVaoFile(Trong *head)
{
    FILE *tempFile = fopen("Trong_temp.txt", "w"); // Mở file tạm
    if (tempFile == NULL)
    {
        printf("Khong the mo file tam de ghi!\n");
        return;
    }

    Trong *current = head;
    while (current != NULL)
    {
        fprintf(tempFile, "%s,%s,%s,%s,%d,%d,%d,%d,%s\n",
                current->MaTro,
                current->HoTenCT,
                current->Diachitro,
                current->SDTCT,
                current->DienSo,
                current->NuocSo,
                current->DichVuThang,
                current->TienPhong,
                current->TrangThai);
        current = current->next;
    }

    fclose(tempFile);

    // Thay thế file gốc bằng file tạm
    remove("Trong.txt");                   // Xóa file gốc
    rename("Trong_temp.txt", "Trong.txt"); // Đổi tên file tạm thành file gốc
    printf("Ghi danh sach trong thanh cong vao file Trong.txt.\n");
}
// Danh Sach Menu
void mainMenu()
{
    printf("\n===== DANG NHAP VAO HE THONG =====\n");
    printf("----------------***-----------------\n");
    printf("1. DANG KY TAI KHOAN \n");
    printf("2. DANG NHAP \n");
    printf("0. Thoat\n");
    printf("Lua chon cua ban: ");
}
void ADMenu()
{
    printf("====================================\n");
    printf("                MENU\n");
    printf("====================================\n");
    printf("1. Them danh sach phong tro\n");
    printf("2. Xem danh sach phong tro\n");
    printf("3. Tim phong tro\n");
    printf("4. Xoa phong tro\n");
    printf("5. Them thong bao\n");
    printf("6. Xem phan hoi\n");
    printf("0. Thoat\n");
    printf("Chon tuy chon cua ban (0-6): ");
}
void Trangtt()
{
    hieuung("\nThong tin, gioi thieu: ");
    hieuung("\nDay la mot chuong trinh cua nhom gom 4 thanh vien: ");
    hieuung("\n* Mai Van Truong (26A4041674) ");
    hieuung("\n* Phan Quang Huy (26A4041190)");
    hieuung("\n* Dang Truong Duy (26A4040718)");
    hieuung("\n* Ngo Quang Manh (26A4041645)");
    hieuung("\nChao mung ban den voi he thong quan ly phong tro!");
    printf("\n-----------------------------------------------------\n");
}
void TPMenu()
{
    printf("====================================\n");
    printf("             Them Tro\n");
    printf("====================================\n");
    printf("1. Them tro trong.\n");
    printf("2. Them tro Thue.\n");
    printf("0. Quay lai Menu.\n");
    printf("Chon tuy chon cua ban (0-2): ");
}
void InMenu()
{
    printf("====================================\n");
    printf("             In DS_Tro\n");
    printf("====================================\n");
    printf("1. In Tro trong.\n");
    printf("2. In Tro Thue.\n");
    printf("0. Quay lai Menu.\n");
    printf("Chon tuy chon cua ban (0-2): ");
}
void TimPMenu()
{
    printf("====================================\n");
    printf("               Tim Tro\n");
    printf("====================================\n");
    printf("1. Tim Tro trong.\n");
    printf("2. Tim Tro Thue.\n");
    printf("0. Quay lai Menu.\n");
    printf("Chon tuy chon cua ban (0-2): ");
}
void KTMenu()
{
    printf("====================================\n");
    printf("                 MENU\n");
    printf("====================================\n");
    // XemThongBao();
    printf("1. Xem thong tin phong tro trong\n");
    printf("2. Tim thong tin phong tro cua ban\n");
    printf("3. Gui phan hoi\n");
    printf("4. Xem thong bao\n");
    printf("5. Xem Hoa Don \n");
    printf("0. Thoat\n");
    printf("Chon tuy chon cua ban (0-5): ");
}
void TimPhong(Thue *head)
{
    char maTro[15];
    printf("Nhap ma phong cua ban: ");
    fflush(stdin);
    gets(maTro);
    maTro[strcspn(maTro, "\n")] = '\0';
    Thue *current = head;
    int i = 1;
    while (current != NULL)
    {
        if (strcmp(current->MaTro, maTro) == 0)
        {
            // Nếu mã trọ tìm thấy
            gotoxy(2, 1);
            hieuung("\nThong tin phong tro:\n");
            gotoxy(2, 3);
            printf("Ma phong");
            gotoxy(13, 3);
            printf("Ho ten khach thue");
            gotoxy(35, 3);
            printf("Dia chi tro");
            gotoxy(50, 3);
            printf("SDT");
            gotoxy(63, 3);
            printf("Tien dien");
            gotoxy(77, 3);
            printf("Tien nuoc");
            gotoxy(92, 3);
            printf("Tien dich vu");
            gotoxy(110, 3);
            printf("Tien Phong");
            gotoxy(130, 3);
            printf("Tong tien");
            gotoxy(149, 3);
            printf("Trang thai");
            gotoxy(2, i + 3);
            printf("%s", current->MaTro);
            gotoxy(13, i + 3);
            printf("%s", current->HoTenKT);
            gotoxy(35, i + 3);
            printf("%s", current->Diachitro);
            gotoxy(50, i + 3);
            printf("%s", current->SDTKT);
            gotoxy(63, i + 3);
            printf("%d VND", current->Dien);
            gotoxy(77, i + 3);
            printf("%d VND", current->Nuoc);
            gotoxy(92, i + 3);
            printf("%d VND", current->DichVu);
            gotoxy(110, i + 3);
            printf("%d VND", current->TienPhong);
            gotoxy(130, i + 3);
            printf("%d VND", current->TongTien);
            gotoxy(149, i + 3);
            printf("%s", current->TrangThai);
            return;
        }
        current = current->next;
    }

    // Nếu không tìm thấy mã trọ
    printf("Khong tim thay phong voi ma tro: %s\n", maTro);
}
void TimPhongTrong(Trong *head)
{
    char maTro[15];
    printf("Nhap ma phong can tim: ");
    fflush(stdin);
    gets(maTro);
    maTro[strcspn(maTro, "\n")] = '\0'; // Xóa ký tự xuống dòng nếu có

    Trong *current = head;
    while (current != NULL)
    {
        if (strcmp(current->MaTro, maTro) == 0)
        {
            if (strcmp(current->TrangThai, "Trong") == 0)
            {
                // Nếu tìm thấy phòng trống
                gotoxy(2, 1);
                printf("Thong tin phong tro:");
                gotoxy(2, 3);
                hieuung("-------------------------------");
                gotoxy(2, 5);
                printf("Ma phong");
                gotoxy(13, 5);
                printf("Ho ten khach thue");
                gotoxy(35, 5);
                printf("Dia chi tro");
                gotoxy(50, 5);
                printf("SDT");
                gotoxy(63, 5);
                printf("Tien dien");
                gotoxy(77, 5);
                printf("Tien nuoc");
                gotoxy(92, 5);
                printf("Tien dich vu");
                gotoxy(110, 5);
                printf("Tien Phong");
                gotoxy(130, 5);
                printf("Tong tien");
                gotoxy(149, 5);
                printf("Trang thai");
                gotoxy(2, 6);
                printf("%s\n", current->MaTro);
                gotoxy(13, 6);
                printf("%s\n", current->HoTenCT);
                gotoxy(35, 6);
                printf("Dia chi: %s\n", current->Diachitro);
                gotoxy(50, 6);
                printf("%s\n", current->SDTCT);
                gotoxy(63, 6);
                printf("%d VND\n", current->DienSo);
                gotoxy(77, 6);
                printf("%d\ VND\n", current->NuocSo);
                gotoxy(92, 6);
                printf("%d VND\n", current->DichVuThang);
                gotoxy(110, 6);
                printf("%d VND\n", current->TienPhong);
                gotoxy(149, 6);
                printf("%s\n", current->TrangThai);
                return;
            }
            else
            {
                // Nếu phòng không ở trạng thái trống
                printf("Phong co ma tro: %s khong trong trang thai trong!\n", maTro);
                return;
            }
        }
        current = current->next;
    }

    // Nếu không tìm thấy mã trọ
    printf("Khong tim thay phong voi ma tro: %s\n", maTro);
}
void GuiPhanHoi(Trong *headTrong, Thue *headThue)
{
    FILE *file = fopen("PhanHoi.txt", "a");
    if (file == NULL)
    {
        printf("Khong the mo file PhanHoi.txt!\n");
        return;
    }
    PhanHoi ph;
    char maTro[15];
    int found = 0;
    system("cls");
    while (1)
    {
        printf("Nhap Ma tro cua ban: ");
        fflush(stdin);
        gets(ph.MaTro);
        ph.MaTro[strcspn(ph.MaTro, "\n")] = '\0';
        if (kiemTraMaTroTonTai(headThue, headTrong, ph.MaTro) == 0)
        {
            printf("Ma Tro Khong ton tai! Vui long nhap lai.\n");
        }
        else
        {
            break;
        }
    }
    printf("Nhap ten cua ban: ");
    fgets(ph.HoTenKT, sizeof(ph.HoTenKT), stdin);
    ph.HoTenKT[strcspn(ph.HoTenKT, "\n")] = '\0';
    printf("Nhap SDT: ");
    fgets(ph.SDTKT, sizeof(ph.SDTKT), stdin);
    ph.SDTKT[strcspn(ph.SDTKT, "\n")] = '\0';
    printf("Nhap noi dung phan hoi: ");
    fgets(ph.PhanHoi, sizeof(ph.PhanHoi), stdin);
    ph.PhanHoi[strcspn(ph.PhanHoi, "\n")] = '\0';
    fprintf(file, "%s,%s,%s,%s\n", ph.MaTro, ph.HoTenKT, ph.SDTKT, ph.PhanHoi);
    fclose(file);
    printf("Phan hoi da duoc gui thanh cong!\n");
}
void XemPhanHoi()
{
    FILE *file = fopen("PhanHoi.txt", "r");
    if (file == NULL)
    {
        printf("Khong co phan hoi nao.\n");
        return;
    }
    PhanHoi ph;
    system("cls");
    printf("DANH SACH PHAN HOI\n");
    printf("====================\n");
    while (fscanf(file, "%[^,],%[^,],%[^,],%[^\n]\n", ph.MaTro, ph.HoTenKT, ph.SDTKT, ph.PhanHoi) == 4)
    {
        printf("Ma Tro: %s\n", ph.MaTro);
        printf("Khach Thue: %s\n", ph.HoTenKT);
        printf("SDT: %s\n", ph.SDTKT);
        printf("Phan Hoi: %s\n", ph.PhanHoi);
        printf("------------------------\n");
    }
    fclose(file);
}