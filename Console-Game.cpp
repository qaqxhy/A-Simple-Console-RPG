#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <cwchar>
#include <locale.h>

using namespace std;

const int bllins = 30; // x
const int blcols = 50; // y

string block[3] = {"  ", "██", "◯ "};

char maploaded[bllins][blcols]; // block ID inside

struct PLAYER
{
    int health;
    int attach;
    int level;
    int x;
    int y;
} player;

void printblock(int ID)
{
    switch (ID)
    {
    case 0:
    {
        printf("  ");
        break;
    }
    case 1:
    {
        printf("██");
        break;
    }
    case 2:
    {
        printf("◯ ");
        break;
    }
    default:
    {
        break;
    }
    }
}

void clear()
{
    for (int x = 0; x < bllins + 1; x++)
    {
        cout << '\n';
    }
}

void render()
{
    // clear();
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    // ("HP: %d    AT: %d    LV: %d    Location: %d,%d", player.health, player.attach, player.level, player.x, player.y);
    cout << "HP: " << player.health << "    "
         << "AT: " << player.attach << "    "
         << "LV: " << player.level << "    "
         << "Location: " << player.x << ' ' << player.y;
    // for (int x = 0; x < bllins; x++)
    // {
    //     cout << '\n';
    //     for (int y = 0; y < blcols; y++)
    //     {
    //         if (player.x == x && player.y == y)
    //         {
    //             cout << block[2];
    //         }
    //         else
    //         {
    //             // printblock(maploaded[x][y] - '0');
    //             cout << block[maploaded[x][y] - '0'];
    //         }
    //     }
    // }
}

void save()
{
    ofstream outfile;
    outfile.open("save.dat", ios::out | ios::trunc);
    outfile << player.health << endl
            << player.attach << endl
            << player.level << endl
            << player.x << endl
            << player.y << endl;
    for (int x = 0; x < bllins; x++)
    {
        for (int y = 0; y < blcols; y++)
        {
            outfile << maploaded[x][y];
        }
        outfile << endl;
    }
    outfile.close();
}

void checkSave()
{
    if (player.health <= 0)
    {
        player.health = 100;
    }
    if (player.attach <= 0)
    {
        player.attach = 4;
    }
    for (int x = 0; x < bllins; x++)
    {
        for (int y = 0; y < blcols; y++)
        {
            if (maploaded[x][y] == 0)
            {
                maploaded[x][y] = '0';
            }
        }
    }
    save();
}

void ReadSave()
{
    ifstream infile;
    infile.open("save.dat");
    infile >> player.health >> player.attach >> player.level >> player.x >> player.y;
    for (int x = 0; x < bllins; x++)
    {
        infile >> maploaded[x];
    }
    infile.close();
}

void init()
{
    system("chcp 65001"); // UTF 8 mode
    setlocale(LC_ALL, "en_US.UTF-8");
    // system("color F0");

    setvbuf(stdout, NULL, _IONBF, 0);

    char setWinSize[64];
    sprintf(setWinSize, "mode con cols=%d lines=%d", 2 * blcols, bllins + 1); // 2 char per block, 2 times of width
    system(setWinSize);                                                       // set window size

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;  // Width of each character in the font
    cfi.dwFontSize.Y = 16; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");                                // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi); // set font

    system("cls");

    ReadSave();

    checkSave();

    // ("Preas any key to start!\n");
    cout << "Preas any key to start!";
    std::ios::sync_with_stdio(false); // fast console output
    while (1)
    {
        if (_kbhit())
        {
            _getch();
            render();
            return;
        }
    }
}

void RealTimeLogic()
{
    if (_kbhit())
    {
        int key = _getch();
        switch (key)
        {
        case 27:
        {
            save();
            exit(0);
            break;
        }
        case 102: // F
        {
            player.health++;
            break;
        }
        case 100:
        {
            if (maploaded[player.x][player.y + 1] == '0')
            {
                player.y++;
            }
            break;
        }
        case 97:
        {
            if (maploaded[player.x][player.y - 1] == '0')
            {
                player.y--;
            }
            break;
        }
        case 115:
        {
            if (maploaded[player.x + 1][player.y] == '0')
            {
                player.x++;
            }
            break;
        }
        case 119:
        {
            if (maploaded[player.x - 1][player.y] == '0')
            {
                player.x--;
            }
            break;
        }
        default:
        {
            break;
        }
        }
        render();
    }
}

int main(int argc, char *argv[])
{
    init();
    while (1)
    {
        RealTimeLogic();
    }

    return 0;
}
