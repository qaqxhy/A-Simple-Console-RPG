// THIS IS C++
#include <cwchar>
#include <windows.h>
#include <iostream>

#include <fstream>
#include <conio.h>
#include <locale.h>
#include <fstream>
#include <direct.h>

using namespace std;
int main(int argc, char const *argv[])
{
    system("chcp 65001");

    setlocale(LC_CTYPE, "en_US.UTF-8");
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;  // Width of each character in the font
    cfi.dwFontSize.Y = 16; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    // std::cout << "◯ kk\n";
    system("cls");

    // std::ios::sync_with_stdio(false); // fast console output
    // std::ios_base::sync_with_stdio(false); // fast console output

    // _mkdir("./dat");
    // _mkdir("./dat/map");
    for (int i = 0; i < argc; i++)
    {
        cout << argv[i] << endl;
    }
    return 0;
}
