// THIS IS C++
#include <cwchar>
#include <windows.h>
#include <iostream>

#include <fstream>
#include <conio.h>
#include <locale.h>
#include <fstream>
#include <direct.h>

#define UNDERLINE "\033[4m"

#define CLOSEUNDERLINE "\033[0m"

using namespace std;

static inline int strcmp_asm(const char *cs, const char *ct)
{
    int d0, d1;
    register int __res;
    __asm__ __volatile__(
        "1:\tlodsb\n\t"
        "scasb\n\t"
        "jne 2f\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n\t"
        "xorl %%eax,%%eax\n\t"
        "jmp 3f\n"
        "2:\tsbbl %%eax,%%eax\n\t"
        "orb $1,%%al\n"
        "3:"
        : "=a"(__res), "=&S"(d0), "=&D"(d1)
        : "1"(cs), "2"(ct));
    return __res;
}

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
    // for (int i = 0; i < argc; i++)
    // {
    //     cout << argv[i] << endl;
    // }
    // printf(UNDERLINE);
    // printf(CLOSEUNDERLINE);

    // SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {1,0});
    // printf("◈ ");

    // int arc = 0;
    // char command[123];
    // char arv[10][64];
    // gets(command); // with space
    // char *token;
    // token = strtok(command, " ");
    // while (token != NULL)
    // {
    //     strcpy(arv[arc++], token);

    //     token = strtok(NULL, " ");
    // }
    // for (int i = 0; i < arc; i++)
    // {
    //     puts(arv[i]);
    // }

    // int start = 0;
    // for (int i = 0; i < 123; i++)
    // {
    //     if (command[i] == ' ')
    //     {
    //         strncpy(arv[arc++], command + start, i - start);
    //         start = i + 1;
    //     }
    //     else if( command[i] == 0)
    //     {
    //         break;
    //     }
    // }

    int res = strcmp_asm("123", "123");
    printf("%d", res);
    return 0;
}
