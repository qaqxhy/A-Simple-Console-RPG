/*
Notes:
    !. Use std output ONLY
    !. puts() is faster than printf()
    !. map limit is 30*50, im too lazy to change the window size :)
    !. block is 2 character
    Bulid:
    g++ C:\Users\qaqxhy\CPP\Console-Game\Console-Game-C.cpp -o C:\Users\qaqxhy\CPP\Console-Game\Binary\BIN.exe -O3 -m64 -Wall -static-libgcc -fexec-charset=utf-8 -D__USE_MINGW_ANSI_STDIO

*/

#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <cwchar>
#include <locale.h>
#include <direct.h>

using namespace std;

const int bllins = 30; // x
const int blcols = 50; // y

short GameMode = 0; // 0 Survival 1 Creative (?)

char block[4][64] = {"  ", "██", "◯ ", "<-"};

// char maploaded[bllins][blcols]; // block ID inside

char vram[10000]; // hahaha

struct PLAYER
{
    int health;
    int attach;
    int level;
    int levelsave[100][3]; // passed?    last x    last y
} player;

struct MAP
{
    char map[100][100]; //  --Y
                        // |
                        // X
    int rex;
    int rey;
} maploaded;

bool CheckSeqPos(int x, int y)
{
    if (x < 0 || 29 < x)
    {
        return 0;
    }
    else if (y < 0 || 49 < y)
    {
        return 0;
    }
    else if (maploaded.map[x][y] != '0' && GameMode == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void Clear()
{
    // for (int x = 0; x < bllins + 1; x++)
    // {
    //     printf("\n");
    // }
    puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); // stupid way to clear screen but fast :)
}

void Render()
{
    Clear();
    memset(vram, 0, sizeof(vram));

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    sprintf(vram, "HP: %d    AT: %d    LV: %d    Location: %d,%d\n", player.health, player.attach, player.level, player.levelsave[player.level][1], player.levelsave[player.level][2]);
    for (int x = 0; x < bllins; x++)
    {
        if (GameMode == 1)
        {
            for (int y = 0; y < blcols; y++)
            {
                if (player.levelsave[player.level][1] == x && player.levelsave[player.level][2] == y)
                {
                    if (maploaded.rex == x && maploaded.rey == y)
                    {
                        strcat(vram, block[2]);
                    }
                    else
                    {
                        strcat(vram, block[maploaded.map[x][y] - '0']);
                    }
                    strcat(vram, block[3]);
                }
                else if (maploaded.rex == x && maploaded.rey == y)
                {
                    if (maploaded.rex == player.levelsave[player.level][1] && maploaded.rey == player.levelsave[player.level][2] + 1)
                    {
                        continue;
                    }
                    else
                    {
                        strcat(vram, block[2]);
                    }
                }
                else if (player.levelsave[player.level][1] == x && player.levelsave[player.level][2] + 1 == y)
                {
                    continue;
                }
                else
                {
                    strcat(vram, block[maploaded.map[x][y] - '0']);
                }
            }
        }
        else
        {
            for (int y = 0; y < blcols; y++)
            {
                if (player.levelsave[player.level][1] == x && player.levelsave[player.level][2] == y)
                {
                    strcat(vram, block[2]);
                }
                else
                {
                    strcat(vram, block[maploaded.map[x][y] - '0']);
                }
            }
        }
        strcat(vram, "\n");
    }
    if (GameMode == 0)
    {
        strcat(vram, "WASD: Move    F: Heal    <>: Change Level    Enter: Save    ESC: Save and Quit");
    }
    else if (GameMode == 1)
    {
        strcat(vram, "WASD: Move    <>: Change Level    Enter: Save Map    ESC: Save Map and Quit\n");
        strcat(vram, "0: Clear Block    1: Block    2: Set Res Point");
    }
    puts(vram); // faster than printf and cout
                //! but 1 more \n behinde;
}

void Save()
{
    ofstream outfile;
    if (GameMode == 0)
    {
        outfile.open("./data/save.dat", ios::out | ios::trunc);
        outfile << player.health << endl
                << player.attach << endl
                << player.level << endl;
        for (int i = 0; i < 100; i++)
        {
            outfile << player.levelsave[i][0] << " " << player.levelsave[i][1] << " " << player.levelsave[i][2] << endl;
        }
        outfile.close();
    }

    char maploc[64];
    sprintf(maploc, "./data/maps/map%d.dat", player.level);
    outfile.open(maploc, ios::out | ios::trunc);
    outfile << maploaded.rex << endl
            << maploaded.rey << endl;
    for (int x = 0; x < bllins; x++)
    {
        for (int y = 0; y < blcols; y++)
        {
            outfile << maploaded.map[x][y];
        }
        outfile << endl;
    }
    outfile.close();
}

void CheckSave()
{
    if (player.health <= 0)
    {
        player.health = 100;
    }
    if (player.attach <= 0)
    {
        player.attach = 4;
    }
}

void ReadSave()
{
    ifstream infile;
    infile.open("./data/save.dat");
    infile >> player.health >> player.attach >> player.level;
    for (int i = 0; i < 100; i++)
    {
        infile >> player.levelsave[i][0] >> player.levelsave[i][1] >> player.levelsave[i][2];
    }
    infile.close();
}

void ReadMap()
{
    ifstream infile;
    char maploc[64];
    sprintf(maploc, "./data/maps/map%d.dat", player.level);
    infile.open(maploc);
    infile >> maploaded.rex >> maploaded.rey;
    for (int x = 0; x < bllins; x++)
    {
        infile >> maploaded.map[x];
    }
    infile.close();
}

void CheckMap()
{
    bool legalrespoint = 1;
    if (maploaded.map[maploaded.rex][maploaded.rey] != '0')
    {
        legalrespoint = 0;
    }
    for (int x = 0; x < bllins; x++)
    {
        for (int y = 0; y < blcols; y++)
        {
            if (legalrespoint == 0 && maploaded.map[x][y] == '0')
            {
                maploaded.rex = x;
                maploaded.rey = y;
                legalrespoint = 1;
            }
            if (maploaded.map[x][y] == 0)
            {
                maploaded.map[x][y] = '0';
            }
        }
    }
}

void Init()
{
    system("chcp 65001"); // UTF 8 mode
    setlocale(LC_ALL, "en_US.UTF-8");
    system("color F0");

    setvbuf(stdout, NULL, _IONBF, 0);

    char setWinSize[64];
    if (GameMode == 0)
    {
        sprintf(setWinSize, "mode con cols=%d lines=%d", 2 * (blcols), bllins + 3); // 2 char per block, 2 times of width
    }
    else if (GameMode == 1)
    {
        sprintf(setWinSize, "mode con cols=%d lines=%d", 2 * (blcols + 1), bllins + 4); // 1 extra for the curse
    }
    system(setWinSize); // set window size

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

    _mkdir("./data");
    _mkdir("./data/maps");

    ReadSave();

    CheckSave();

    ReadMap();

    CheckMap();

    Save();

    SetConsoleTitleW(L"Game");
    printf("Preas any key to start!\n");
    std::ios::sync_with_stdio(false); // fast console output
    while (1)
    {
        if (_kbhit())
        {
            _getch();
            Render();
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
        case 27: // ESC
        {
            Save();
            exit(0);
            break;
        }
        case 102: // F
        {
            player.health++;
            break;
        }
        case 100: // WASD
        {
            if (CheckSeqPos(player.levelsave[player.level][1], player.levelsave[player.level][2] + 1))
            {
                player.levelsave[player.level][2]++;
            }
            break;
        }
        case 97:
        {
            if (CheckSeqPos(player.levelsave[player.level][1], player.levelsave[player.level][2] - 1))
            {
                player.levelsave[player.level][2]--;
            }
            break;
        }
        case 115:
        {
            if (CheckSeqPos(player.levelsave[player.level][1] + 1, player.levelsave[player.level][2]))
            {
                player.levelsave[player.level][1]++;
            }
            break;
        }
        case 119:
        {
            if (CheckSeqPos(player.levelsave[player.level][1] - 1, player.levelsave[player.level][2]))
            {
                player.levelsave[player.level][1]--;
            }
            break;
        }
        case 44: // <
        {
            if (player.level - 1 >= 0)
            {
                player.level--;
                ReadMap();
            }
            break;
        }
        case 46: // >
        {
            if (player.level + 1 <= 99)
            {
                player.level++;
                ReadMap();
            }
            break;
        }
        case 13: // Enter
        {
            Save();
            break;
        }
        case 48: // 0
        {
            if (GameMode == 1)
            {
                maploaded.map[player.levelsave[player.level][1]][player.levelsave[player.level][2]] = '0';
            }
            break;
        }
        case 49: // 1
        {
            if (GameMode == 1)
            {
                maploaded.map[player.levelsave[player.level][1]][player.levelsave[player.level][2]] = '1';
            }
            break;
        }
        case 50: // 2
        {
            if (GameMode == 1)
            {
                maploaded.map[player.levelsave[player.level][1]][player.levelsave[player.level][2]] = '0';
                maploaded.rex = player.levelsave[player.level][1];
                maploaded.rey = player.levelsave[player.level][2];
            }
            break;
        }
        case 51:
        {
            if (GameMode == 1)
            {
            }
            break;
        }
        default:
        {
            break;
        }
        }
        Render();
    }
}

int main(int argc, char const *argv[])
{
    if (argc > 1)
    {
        int argcmp = strcmp(argv[1], "-editor");
        if (argcmp == 0)
        {
            GameMode = 1;
        }
    }
    Init();
    while (1)
    {
        RealTimeLogic();
    }

    return 0;
}
