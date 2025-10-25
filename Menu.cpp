#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip> 
#include <cmath>
#include <windows.h>
using namespace std;

// ================= Danh Cho Quan Ly Hoc Sinh =================
struct HocSinh {
    wstring ten;
    double toan = 0.0;
    double ly = 0.0;
    double hoa = 0.0;
    float diemTB = 0.0f;
    wstring xepLoai;
};

class QuanLyHocSinh {
private:
    vector<HocSinh> danhSach;
    wstring tenFile = L"hocsinh.csv";

    // Ham tinh diem trung binh va xep loai
    void tinhDiemVaXepLoai(HocSinh& hs) {
        hs.diemTB = (hs.toan + hs.ly + hs.hoa) / 3.0;

        if (hs.diemTB >= 9) hs.xepLoai = L"XUAT SAC";
        else if (hs.diemTB >= 8) hs.xepLoai = L"GIOI";
        else if (hs.diemTB >= 6.5) hs.xepLoai = L"KHA";
        else if (hs.diemTB >= 5) hs.xepLoai = L"TRUNG BINH";
        else hs.xepLoai = L"YEU";
    }

    // Ham chuan hoa ten (xoa khoang trang thua)
    wstring chuanHoaTen(wstring ten) {
        wstringstream ss(ten);
        wstring word, result;
        while (ss >> word) {
            if (!result.empty()) result += L" ";
            result += word;
        }
        return result;
    }

   
    string ws2s(const wstring& ws) {
        return string(ws.begin(), ws.end());
    }

  
    wstring s2ws(const string& s) {
        return wstring(s.begin(), s.end());
    }

public:
    // ==================== DOC FILE CSV ====================
    void docFileCSV() {
        string filename = ws2s(tenFile);
        ifstream file(filename);
        if (!file.is_open()) {
            wcout << L"Chua co file du lieu, tao moi..." << endl;
            return;
        }

        danhSach.clear();
        string line;

        // DOC DONG DAU TIEN - HEADER
        getline(file, line);

        // DOC CAC DONG DU LIEU
        int soDong = 0;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string token;
            HocSinh hs;

            try {
                // DOC TEN
                getline(ss, token, ',');
                hs.ten = chuanHoaTen(s2ws(token));

                // DOC DIEM TOAN
                getline(ss, token, ',');
                hs.toan = stod(token);

                // DOC DIEM LY
                getline(ss, token, ',');
                hs.ly = stod(token);

                // DOC DIEM HOA
                getline(ss, token, ',');
                hs.hoa = stod(token);

                // TINH LAI DIEM TRUNG BINH VA XEP LOAI
                tinhDiemVaXepLoai(hs);

                // THEM VAO DANH SACH
                danhSach.push_back(hs);
                soDong++;

            }
            catch (const exception& e) {
                wcout << L"Loi doc dong: " << e.what() << endl;
            }
        }

        file.close();
        wcout << L"Da doc " << soDong << L" hoc sinh tu file: " << tenFile << endl;
    }

    // ==================== GHI FILE CSV ====================
    void ghiFileCSV() {
        string filename = ws2s(tenFile);
        ofstream file(filename);
        if (!file.is_open()) {
            wcout << L"Loi mo file de ghi!" << endl;
            return;
        }

        // GHI DONG HEADER
        file << "Ten,Toan,Ly,Hoa,DiemTB,XepLoai" << endl;

        // GHI TUNG DONG DU LIEU
        for (const auto& hs : danhSach) {
            file << ws2s(hs.ten) << ","
                << hs.toan << ","
                << hs.ly << ","
                << hs.hoa << ","
                << fixed << setprecision(2) << hs.diemTB << ","
                << ws2s(hs.xepLoai) << endl;
        }

        file.close();
        wcout << L"Da luu " << danhSach.size() << L" hoc sinh vao file: " << tenFile << endl;
    }

    // ==================== THEM HOC SINH ====================
    void themHocSinh() {
        HocSinh hs;

        wcout << L"Nhap ten hoc sinh: ";
        wcin.ignore();
        getline(wcin, hs.ten);
        hs.ten = chuanHoaTen(hs.ten);

        wcout << L"Nhap diem Toan: ";
        wcin >> hs.toan;
        wcout << L"Nhap diem Ly: ";
        wcin >> hs.ly;
        wcout << L"Nhap diem Hoa: ";
        wcin >> hs.hoa;

        // KIEM TRA DIEM HOP LE
        if (hs.toan < 0 || hs.toan > 10 || hs.ly < 0 || hs.ly > 10 || hs.hoa < 0 || hs.hoa > 10) {
            wcout << L"Loi: Diem phai tu 0 den 10!" << endl;
            return;
        }

        tinhDiemVaXepLoai(hs);
        danhSach.push_back(hs);

        wcout << L"Da them hoc sinh: " << hs.ten << L" - DTB: " << fixed << setprecision(2) << hs.diemTB << L" - " << hs.xepLoai << endl;
    }

    // ==================== THEM NHIEU HOC SINH CUNG LUC ====================
    void themNhieuHocSinh() {
        wcout << L"\n=== THEM NHIEU HOC SINH CUNG LUC ===" << endl;

        // Mo hop thoai chon file
        OPENFILENAMEW ofn;
        wchar_t szFile[260] = L"";

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
        ofn.lpstrFilter = L"Text Files\0*.txt\0CSV Files\0*.csv\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        wcout << L"Dang mo hop thoai chon file..." << endl;

        if (GetOpenFileNameW(&ofn)) {
            wstring tenFile = ofn.lpstrFile;
            wcout << L"Da chon file: " << tenFile << endl;

            
            string fileA(tenFile.begin(), tenFile.end());
            ifstream file(fileA);

            if (!file.is_open()) {
                wcout << L"Khong the mo file!" << endl;
                return;
            }

            string line;
            int soHocSinhThem = 0;
            int soDongLoi = 0;

            // Doc tung dong
            while (getline(file, line)) {
                if (line.empty()) continue;

                stringstream ss(line);
                string token;
                HocSinh hs;

                try {
                    // Doc ten
                    if (!getline(ss, token, ',')) continue;
                    hs.ten = chuanHoaTen(s2ws(token));

                    // Doc diem toan
                    if (!getline(ss, token, ',')) continue;
                    hs.toan = stod(token);

                    // Doc diem ly
                    if (!getline(ss, token, ',')) continue;
                    hs.ly = stod(token);

                    // Doc diem hoa
                    if (!getline(ss, token, ',')) continue;
                    hs.hoa = stod(token);

                    // Kiem tra diem hop le
                    if (hs.toan < 0 || hs.toan > 10 || hs.ly < 0 || hs.ly > 10 || hs.hoa < 0 || hs.hoa > 10) {
                        wcout << L"Loi: Diem khong hop le cho hoc sinh: " << hs.ten << endl;
                        soDongLoi++;
                        continue;
                    }

                    // Tinh diem trung binh va xep loai
                    tinhDiemVaXepLoai(hs);

                    // Them vao danh sach
                    danhSach.push_back(hs);
                    soHocSinhThem++;

                    wcout << L"Da them: " << hs.ten << L" - DTB: " << fixed << setprecision(2) << hs.diemTB << endl;

                }
                catch (const exception& e) {
                    wcout << L"Loi doc dong: " << e.what() << endl;
                    soDongLoi++;
                }
            }

            file.close();
            wcout << L"\n=== KET QUA NHAP FILE ===" << endl;
            wcout << L"So hoc sinh them thanh cong: " << soHocSinhThem << endl;
            wcout << L"So dong bi loi: " << soDongLoi << endl;
            wcout << L"Tong so hoc sinh hien tai: " << danhSach.size() << endl;

        }
        else {
            wcout << L"Nguoi dung da huy chon file." << endl;
        }
    }

    // ==================== HIEN THI DANH SACH ====================
    void hienThiDanhSach() {
        if (danhSach.empty()) {
            wcout << L"Danh sach trong!" << endl;
            return;
        }

        wcout << L"\n=== DANH SACH HOC SINH ===" << endl;
        wcout << L"STT | Ten hoc sinh       | Toan  | Ly    | Hoa   | DTB   | Xep loai" << endl;
        wcout << L"----|--------------------|-------|-------|-------|-------|----------" << endl;

        for (int i = 0; i < danhSach.size(); i++) {
            const auto& hs = danhSach[i];
            wprintf(L"%3d | %-18s | ", i + 1, hs.ten.c_str());

            wcout << fixed << setprecision(2);
            wcout << setw(5) << hs.toan << L" | "
                << setw(5) << hs.ly << L" | "
                << setw(5) << hs.hoa << L" | "
                << setw(5) << hs.diemTB << L" | "
                << hs.xepLoai << endl;
        }
    }

    // ==================== TIM KIEM HOC SINH ====================
    void timKiemHocSinh() {
        if (danhSach.empty()) {
            wcout << L"Danh sach trong!" << endl;
            return;
        }

        wstring tenCanTim;
        wcout << L"Nhap ten can tim: ";
        wcin.ignore();
        getline(wcin, tenCanTim);
        tenCanTim = chuanHoaTen(tenCanTim);

        bool found = false;
        wcout << L"\n=== KET QUA TIM KIEM ===" << endl;

        for (const auto& hs : danhSach) {
            // TIM KIEM KHONG PHAN BIET HOA THUONG
            wstring tenHS = hs.ten;
            transform(tenHS.begin(), tenHS.end(), tenHS.begin(), ::towlower);
            transform(tenCanTim.begin(), tenCanTim.end(), tenCanTim.begin(), ::towlower);

            if (tenHS.find(tenCanTim) != wstring::npos) {
                wcout << L" - " << hs.ten << L" | Toan: " << hs.toan
                    << L" | Ly: " << hs.ly << L" | Hoa: " << hs.hoa
                    << L" | DTB: " << hs.diemTB << L" | " << hs.xepLoai << endl;
                found = true;
            }
        }

        if (!found) {
            wcout << L"Khong tim thay hoc sinh: " << tenCanTim << endl;
        }
    }

    // ==================== XOA HOC SINH ====================
    void xoaHocSinh() {
        if (danhSach.empty()) {
            wcout << L"Danh sach trong!" << endl;
            return;
        }

        hienThiDanhSach();
        int stt;
        wcout << L"Nhap STT hoc sinh can xoa: ";
        wcin >> stt;

        if (stt < 1 || stt > danhSach.size()) {
            wcout << L"STT khong hop le!" << endl;
            return;
        }

        wstring tenXoa = danhSach[stt - 1].ten;
        danhSach.erase(danhSach.begin() + (stt - 1));
        wcout << L"Da xoa hoc sinh: " << tenXoa << endl;
    }

    // ==================== SAP XEP DANH SACH ====================
    void sapXepTheoDiem() {
        if (danhSach.empty()) {
            wcout << L"Danh sach trong!" << endl;
            return;
        }

        sort(danhSach.begin(), danhSach.end(),
            [](const HocSinh& a, const HocSinh& b) {
                return a.diemTB > b.diemTB; // Giam dan
            });

        wcout << L"Da sap xep theo diem trung binh giam dan!" << endl;
    }
};
// ================= Danh cho phep tinh phan so =================
struct PhanSo {
    int tu;
    int mau;
};
// ================= KHAI BAO HAM =================
bool sochinhphuong(float x);
bool songuyento(float x);
bool songuyen(float x);
int USCLN(int a, int b);
int BSCNN(int a, int b);
bool kiemtragio(int giobatdau, int gioketthuc);
int tinhsogio(int giobatdau, int gioketthuc);
double tinhtiengio(int sogio);
double apdunggiamgia(double tongtien, int giobatdau, bool& daduocgiamgia);
void tiendien();
void doitien();
double nhaptienvay();
double tinhlaisuat(double tienconlai);
double tiengocphaitramoithang(double a);
double sotienconlaisaukhitrulaigoc(double a, double tiengoc);
double tongtiencanphaitra(double laisuat, double tiengoc);
void vaytienmuaxe();
void gamelott();
PhanSo nhapPhanSo();
void hienThi(PhanSo ps);
PhanSo cong(PhanSo a, PhanSo b);
PhanSo tru(PhanSo a, PhanSo b);
PhanSo nhan(PhanSo a, PhanSo b);
PhanSo chia(PhanSo a, PhanSo b);

// ================= Main =================
int main() {
    srand(time(0));
    int choice;

    do {
        cout << "\n=========================================" << endl;
        cout << "|        HAY CHON CHUC NANG            |" << endl;
        cout << "=========================================" << endl;
        cout << "| 1. Kiem tra so nguyen                |" << endl;
        cout << "| 2. Tim USCLN va BSCNN cua 2 so       |" << endl;
        cout << "| 3. Tinh tien quan karaoke            |" << endl;
        cout << "| 4. Tinh tien dien                    |" << endl;
        cout << "| 5. Doi tien                          |" << endl;
        cout << "| 6. Tinh tien lai 12 thang            |" << endl;
        cout << "| 7. Vay tien mua xe                   |" << endl;
        cout << "| 8. Quan ly hoc sinh                  |" << endl;
		cout << "| 9. Game Lottery (So xo)              |" << endl;
		cout << "| 10.Tinh Phan So                      |" << endl;
        cout << "| 0. Thoat                             |" << endl;
        cout << "=========================================" << endl;
        cout << "Nhap lua chon (1-10): ";
        cin >> choice;

        switch (choice) {
        case 1: {
            float x;
            cout << "\n--- KIEM TRA SO NGUYEN ---" << endl;
            cout << "Nhap mot so bat ky: ";
            cin >> x;

            if (songuyento(x))
                cout << x << " la so nguyen to" << endl;
            else
                cout << x << " khong phai so nguyen to" << endl;

            if (sochinhphuong(x))
                cout << x << " la so chinh phuong" << endl;
            else
                cout << x << " khong phai so chinh phuong" << endl;

            if (songuyen(x))
                cout << x << " la so nguyen" << endl;
            else
                cout << x << " khong phai so nguyen" << endl;
            break;
        }

        case 2: {
            int x, y;
            cout << "\n--- TIM USCLN & BSCNN ---" << endl;
            cout << "Nhap so thu nhat: ";
            cin >> x;
            cout << "Nhap so thu hai:  ";
            cin >> y;

            int uscln = USCLN(x, y);
            int bscnn = BSCNN(x, y);

            cout << "\nKET QUA:" << endl;
            cout << "USCLN(" << x << "," << y << ") = " << uscln << endl;
            cout << "BSCNN(" << x << "," << y << ") = " << bscnn << endl;
            break;
        }

        case 3: {
            int giobatdau, gioketthuc;
            cout << "\n--- TINH TIEN KARAOKE ---" << endl;
            cout << "Nhap gio bat dau: ";
            cin >> giobatdau;
            cout << "Nhap gio ket thuc: ";
            cin >> gioketthuc;

            if (!kiemtragio(giobatdau, gioketthuc)) {
                cout << "Gio khong hop le! Quan hoat dong tu 12h den 23h" << endl;
                break;
            }

            int sogio = tinhsogio(giobatdau, gioketthuc);
            double tongtien = tinhtiengio(sogio);
            bool daduocgiamgia = false;
            double tiencuoicung = apdunggiamgia(tongtien, giobatdau, daduocgiamgia);

            cout << "\n--- HOA DON THANH TOAN ---" << endl;
            cout << "So gio hat: " << sogio << " gio" << endl;
            cout << "Tong tien:  " << fixed << setprecision(0) << tiencuoicung << " VND";

            if (daduocgiamgia) {
                cout << " (Da duoc giam gia 10%)" << endl;
            }
            else {
                cout << endl;
            }
            break;
        }

        case 4: {
            tiendien();
            break;
        }

        case 5: {
            doitien();
            break;
        }

        case 6: {
            double sotien = nhaptienvay();
            double tiengoc = tiengocphaitramoithang(sotien);
            double tienconlai = sotien;
            cout << fixed << setprecision(0);
            cout << "\n=== BANG TINH LAI VAY 12 THANG ===" << endl;

            cout << left << setw(8) << "Ky han" << "|"
                << setw(14) << "Lai phai tra" << "|"
                << setw(13) << "Goc phai tra" << "|"
                << setw(19) << "So tien phai tra" << "|"
                << setw(17) << "So tien con lai" << endl;
            cout << "--------|--------------|-------------|-------------------|----------------" << endl;

            for (int thang = 1; thang <= 12; thang++) {
                double lai = tinhlaisuat(tienconlai);
                double tongtra = tongtiencanphaitra(lai, tiengoc);

                cout << left << setw(8) << thang << "|"
                    << right << setw(14) << lai << "|"
                    << setw(13) << tiengoc << "|"
                    << setw(19) << tongtra << "|"
                    << setw(17) << tienconlai << endl;

                tienconlai = sotienconlaisaukhitrulaigoc(tienconlai, tiengoc);
            }
            break;
        }

        case 7: {
            vaytienmuaxe();
            break;

        }

        case 8: {
            QuanLyHocSinh ql;
            int luaChon;

            // TU DONG DOC DU LIEU KHOI DONG
            ql.docFileCSV();

            do {
                cout << "\n=== QUAN LY HOC SINH ===" << endl;
                cout << "1. Them hoc sinh" << endl;
                cout << "2. Them nhieu hoc sinh (chon file)" << endl;
                cout << "3. Hien thi danh sach" << endl;
                cout << "4. Tim kiem hoc sinh" << endl;
                cout << "5. Xoa hoc sinh" << endl;
                cout << "6. Sap xep theo diem" << endl;
                cout << "7. Luu du lieu" << endl;
                cout << "0. Thoat" << endl;
                cout << "Chon chuc nang: ";
                cin >> luaChon;

                switch (luaChon) {
                case 1: ql.themHocSinh(); break;
                case 2: ql.themNhieuHocSinh(); break;
                case 3: ql.hienThiDanhSach(); break;
                case 4: ql.timKiemHocSinh(); break;
                case 5: ql.xoaHocSinh(); break;
                case 6: ql.sapXepTheoDiem(); break;
                case 7: ql.ghiFileCSV(); break;
                case 0:
                    ql.ghiFileCSV(); // TU DONG LUU KHI THOAT
                    cout << "Tam biet!" << endl;    
                    break;
                default:
                    cout << "Lua chon khong hop le!" << endl;
                }
            } while (luaChon != 0);

            break;
        }
        case 9: {
            gamelott();
            break;
		}
        case 10: {
            cout << "\n--- TINH PHAN SO ---" << endl;
            cout << "Nhap phan so thu nhat:" << endl;
            PhanSo ps1 = nhapPhanSo();
            cout << "Nhap phan so thu hai:" << endl;
            PhanSo ps2 = nhapPhanSo();

            cout << "\n=== KET QUA ===" << endl;
            cout << "Phan so 1: ";
            hienThi(ps1);
            cout << endl;
            cout << "Phan so 2: ";
            hienThi(ps2);
            cout << endl;

            PhanSo tong = cong(ps1, ps2);
            PhanSo hieu = tru(ps1, ps2);
            PhanSo tich = nhan(ps1, ps2);
            PhanSo thuong = chia(ps1, ps2);

            cout << "Tong: ";
            hienThi(tong);
            cout << endl;
            cout << "Hieu: ";
            hienThi(hieu);
            cout << endl;
            cout << "Tich: ";
            hienThi(tich);
            cout << endl;
            cout << "Thuong: ";
            hienThi(thuong);
            cout << endl;
            break;
        }

        case 0:
            cout << "\nThoat chuong trinh! Cam on ban da su dung!" << endl;
            break;

        default:
            cout << "\nLua chon khong hop le! Vui long chon 1-10" << endl;
        }
    } while (choice != 0);

    return 0;
}

// ================= Chuc Nang =================

bool songuyento(float x) {
    if (!songuyen(x)) return false;

    bool lasonguyento = true;
    if (x < 2) {
        lasonguyento = false;
    }
    else {
        for (int i = 2; i < x; i++) {
            if ((int)x % i == 0) {
                lasonguyento = false;
                break;
            }
        }
    }
    return lasonguyento;
}

bool sochinhphuong(float x) {
    if (!songuyen(x)) return false;

    bool lasochinhphuong = false;
    if (x < 0) return false;

    for (float i = 0; i <= x; i++) {
        if (i * i == x) {
            lasochinhphuong = true;
            break;
        }
    }
    return lasochinhphuong;
}

bool songuyen(float x) {
    return (x == (int)x);
}

int USCLN(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int BSCNN(int a, int b) {
    return (a * b) / USCLN(a, b);
}

bool kiemtragio(int giobatdau, int gioketthuc) {
    if (giobatdau < 12 || giobatdau > 23 ||
        gioketthuc < 12 || gioketthuc > 23) {
        return false;
    }
    if (gioketthuc <= giobatdau) {
        return false;
    }
    return true;
}

int tinhsogio(int giobatdau, int gioketthuc) {
    return gioketthuc - giobatdau;
}

double tinhtiengio(int sogio) {
    double tongtien = 0;
    if (sogio <= 3) {
        tongtien = sogio * 150000;
    }
    else {
        tongtien = 3 * 150000 + (sogio - 3) * 150000 * 0.7;
    }
    return tongtien;
}

double apdunggiamgia(double tongtien, int giobatdau, bool& daduocgiamgia) {
    daduocgiamgia = false;
    if (giobatdau >= 14 && giobatdau <= 17) {
        tongtien = tongtien * 0.9;
        daduocgiamgia = true;
    }
    return tongtien;
}

void tiendien() {
    cout << "\n--- TINH TIEN DIEN ---" << endl;
    float sd, sotien;
    float b1 = 1.678, b2 = 1.734, b3 = 2.014, b4 = 2.536, b5 = 2.834, b6 = 2.927;
    cout << "Nhap so dien (kWh): ";
    cin >> sd;

    if (sd <= 50) {
        sotien = sd * b1;
    }
    else if (sd <= 100) {
        sotien = 50 * b1 + (sd - 50) * b2;
    }
    else if (sd <= 200) {
        sotien = 50 * b1 + 50 * b2 + (sd - 100) * b3;
    }
    else if (sd <= 300) {
        sotien = 50 * b1 + 50 * b2 + 100 * b3 + (sd - 200) * b4;
    }
    else if (sd <= 400) {
        sotien = 50 * b1 + 50 * b2 + 100 * b3 + 100 * b4 + (sd - 300) * b5;
    }
    else {
        sotien = 50 * b1 + 50 * b2 + 100 * b3 + 100 * b4 + 100 * b5 + (sd - 400) * b6;
    }
    cout << fixed << setprecision(0);
    cout << "So tien can phai tra la: " << sotien * 1000 << " VND" << endl;
}

#define MAX 12
void doitien() {
    cout << "\n--- DOI TIEN ---" << endl;
    int loaitien[MAX] = { 500, 200, 100, 50, 20, 10, 5, 2, 1 };
    int sotien, soluong;
    cout << "Nhap menh gia tien can doi: ";
    cin >> sotien;

    cout << "\nKet qua doi tien:" << endl;
    for (int i = 0; i < MAX; i++) {
        if (sotien >= loaitien[i]) {
            soluong = sotien / loaitien[i];
            sotien = sotien % loaitien[i];
            cout << "Co " << soluong << " to tien " << loaitien[i] << " dong" << endl;
        }
    }
}

double nhaptienvay() {
    double a;
    cout << "Hay nhap so tien muon vay: ";
    cin >> a;
    return a;
}

double tinhlaisuat(double tienconlai) {
    double laisuat = tienconlai * 0.05;
    return round(laisuat * 10) / 10;
}

double tiengocphaitramoithang(double a) {
    double tiengoc = 0;
    tiengoc = a / 12;
    return tiengoc;
}

double sotienconlaisaukhitrulaigoc(double a, double tiengoc) {
    double tienconlai = a - tiengoc;
    return tienconlai;
}

double tongtiencanphaitra(double laisuat, double tiengoc) {
    double tongtra = 0;
    tongtra = laisuat + tiengoc;
    return tongtra;
}

void vaytienmuaxe() {
    cout << "\n--- VAY TIEN MUA XE ---" << endl;

    const double TIEN_VAY = 500000000;
    const int THOI_HAN_VAY = 24 * 12;
    const double LAI_SUAT_NAM = 7.2;

    int phan_tram_vay;
    cout << "Nhap phan tram vay toi da (vi du: 80 - la tra truoc 20%): ";
    cin >> phan_tram_vay;

    if (phan_tram_vay < 0 || phan_tram_vay > 100) {
        cout << "Phan tram vay khong hop le! Vui long nhap tu 0 den 100." << endl;
        return;
    }

    double so_tien_vay = TIEN_VAY * phan_tram_vay / 100;
    double so_tien_tra_truoc = TIEN_VAY - so_tien_vay;

    double lai_suat_thang = LAI_SUAT_NAM / 100 / 12;

    double tien_tra_hang_thang = (so_tien_vay * lai_suat_thang * pow(1 + lai_suat_thang, THOI_HAN_VAY))
        / (pow(1 + lai_suat_thang, THOI_HAN_VAY) - 1);

    cout << fixed << setprecision(0);
    cout << "\n=== KET QUA TINH TOAN VAY MUA XE ===" << endl;
    cout << "Gia tri xe: " << TIEN_VAY << " VND" << endl;
    cout << "Phan tram vay: " << phan_tram_vay << "%" << endl;
    cout << "So tien vay: " << so_tien_vay << " VND" << endl;
    cout << "Thoi han vay: 24 nam (" << THOI_HAN_VAY << " thang)" << endl;
    cout << "Lai suat nam: " << LAI_SUAT_NAM << "%" << endl;
    cout << "----------------------------------------" << endl;
    cout << "SO TIEN PHAI TRA LAN DAU: " << so_tien_tra_truoc << " VND" << endl;
    cout << "SO TIEN PHAI TRA HANG THANG: " << tien_tra_hang_thang << " VND" << endl;
    cout << "========================================" << endl;

    cout << "\n=== BANG TINH TOAN CHI TIET (12 thang dau) ===" << endl;
    cout << left << setw(6) << "Thang" << "|"
        << right << setw(15) << "Lai phai tra" << "|"
        << setw(15) << "Goc phai tra" << "|"
        << setw(20) << "So tien phai tra" << "|"
        << setw(18) << "So tien con lai" << endl;
    cout << "------|---------------|---------------|--------------------|------------------" << endl;

    double tien_con_lai = so_tien_vay;

    for (int thang = 1; thang <= 12; thang++) {
        double lai_thang = tien_con_lai * lai_suat_thang;
        double goc_thang = tien_tra_hang_thang - lai_thang;
        double tong_tra_thang = tien_tra_hang_thang;

        cout << left << setw(6) << thang << "|"
            << right << setw(15) << fixed << setprecision(0) << lai_thang << "|"
            << setw(15) << goc_thang << "|"
            << setw(20) << tong_tra_thang << "|"
            << setw(18) << tien_con_lai << endl;

        tien_con_lai -= goc_thang;
        if (tien_con_lai < 0) tien_con_lai = 0;
    }
}

void gamelott() {
    printf("\nBan da chon Chuc nang so 9\n");

    int i, so1, so2, count = 0;
    int kq, ketqua[2];

    so1 = 0;
    so2 = 0;
    count = 0;

    for (i = 0; i < 2; i++) {
        ketqua[i] = 0;
    }

    printf("Nhap so 1: ");
    scanf_s("%d", &so1);

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("Nhap so 2: ");
    scanf_s("%d", &so2);

    while ((c = getchar()) != '\n' && c != EOF) {}

    for (i = 0; i < 2; i++) {
        kq = 1 + rand() % 15;
        ketqua[i] = kq;
    }

    for (i = 0; i < 2; i++) {
        if (so1 == ketqua[i]) {
            count++;
        }
        if (so2 == ketqua[i]) {
            count++;
        }
    }

    printf("\nKet qua so xo hom nay la!");
    printf("\nSo 1 = %d", ketqua[0]);
    printf("\nSo 2 = %d", ketqua[1]);

    if (count == 2) {
        printf("\nChuc mung ban da trung giai dac biet!");
        printf("\nGia tri giai thuong: 65 ti VND");
    }
    else if (count == 1) {
        printf("\nChuc mung ban da trung giai nhat!");
        printf("\nGia tri giai thuong: 500 trieu VND");
    }
    else {
        printf("\nCHUC BAN MAY MAN LAN SAU!");
    }

    printf("\n\n");
}

PhanSo nhapPhanSo() {
    PhanSo ps;
    cout << "Nhap tu so: ";
    cin >> ps.tu;

    cout << "Nhap mau so: ";
    cin >> ps.mau;

    while (ps.mau == 0) {
        cout << "Mau so phai khac 0! Nhap lai: ";
        cin >> ps.mau;
    }

    return ps;
}

void hienThi(PhanSo ps) {
    cout << ps.tu << "/" << ps.mau;
}


PhanSo cong(PhanSo a, PhanSo b) {
    PhanSo kq;
    kq.tu = a.tu * b.mau + b.tu * a.mau;
    kq.mau = a.mau * b.mau;
    return kq;
}


PhanSo tru(PhanSo a, PhanSo b) {
    PhanSo kq;
    kq.tu = a.tu * b.mau - b.tu * a.mau;
    kq.mau = a.mau * b.mau;
    return kq;
}


PhanSo nhan(PhanSo a, PhanSo b) {
    PhanSo kq;
    kq.tu = a.tu * b.tu;
    kq.mau = a.mau * b.mau;
    return kq;
}


PhanSo chia(PhanSo a, PhanSo b) {
    PhanSo kq;
    kq.tu = a.tu * b.mau;
    kq.mau = a.mau * b.tu;
    return kq;
}
