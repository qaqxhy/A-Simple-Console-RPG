/*
    My code is piece of shit, im gonna rewrite it.
*/
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <cwchar>
#include <locale.h>
#include <direct.h>

#define MP_Height 30
#define MP_Width 50

#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

short GameMode = 0;

const char block[4][16] = {"  ", "██", "◯ "};

const int WinHeight = (MP_Height + 3);
const int WinWidth = (MP_Width * 2);

char vram[10000];

struct PLAYER
{
    int health;
    int attach;
    int level;
    short levelsave[100][3]; // passed?    last x    last y
} player;

struct MAP
{
    char map[100][100]; //  --Y
                        // |
                        // X
    int rex;
    int rey;
} maploaded;

void Render();
bool CheckSeqPos(int x, int y);
void RealTimeLogic();
void Save();
void CheckMap();
void ReadMap();
void CheckSave();
void ReadSave();
void ClearSrc();
void SetWindowSize(int width, int height);
void Init();

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

// Define Functions
void Render()
{
    ClearSrc();
    memset(vram, 0, sizeof(vram));

    sprintf(vram, "HP: %d    AT: %d    LV: %d    Location: %d,%d\n", player.health, player.attach, player.level, player.levelsave[player.level][1], player.levelsave[player.level][2]);
    for (int x = 0; x < MP_Height; x++)
    {
        for (int y = 0; y < MP_Width; y++)
        {
            if (player.levelsave[player.level][1] == x && player.levelsave[player.level][2] == y)
            {
                if (GameMode)
                {
                    // strcat(vram, UNDERLINE);
                    if (x == maploaded.rex && y == maploaded.rey)
                    {
                        strcat(vram, block[2]);
                    }
                    else
                    {
                        strcat(vram, block[maploaded.map[x][y] - '0']);
                    }
                    // strcat(vram, CLOSEUNDERLINE);
                }
                else
                {
                    strcat(vram, block[2]);
                }
            }
            else if (x == maploaded.rex && y == maploaded.rey && GameMode == 1)
            {
                strcat(vram, block[2]);
            }
            else
            {
                strcat(vram, block[maploaded.map[x][y] - '0']);
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
    if (GameMode == 1)
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {short(player.levelsave[player.level][2] * short(2)), short(player.levelsave[player.level][1] + 1)});
    }
}

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
            ClearSrc();
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

void Save()
{
    std::ofstream outfile;
    if (GameMode == 0)
    {
        outfile.open("./data/save.dat", std::ios::out | std::ios::trunc);
        outfile << player.health << std::endl
                << player.attach << std::endl
                << player.level << std::endl;
        for (int i = 0; i < 100; i++)
        {
            outfile << player.levelsave[i][0] << " " << player.levelsave[i][1] << " " << player.levelsave[i][2] << std::endl;
        }
        outfile.close();
    }

    char maploc[64];
    sprintf(maploc, "./data/maps/map%d.dat", player.level);
    outfile.open(maploc, std::ios::out | std::ios::trunc);
    outfile << maploaded.rex << std::endl
            << maploaded.rey << std::endl;
    for (int x = 0; x < MP_Height; x++)
    {
        for (int y = 0; y < MP_Width; y++)
        {
            outfile << maploaded.map[x][y];
        }
        outfile << std::endl;
    }
    outfile.close();
}

void CheckMap()
{
    bool legalrespoint = 1;
    if (maploaded.map[maploaded.rex][maploaded.rey] != '0')
    {
        legalrespoint = 0;
    }
    for (int x = 0; x < MP_Height; x++)
    {
        for (int y = 0; y < MP_Width; y++)
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

void ReadMap()
{
    std::ifstream infile;
    char maploc[64];
    sprintf(maploc, "./data/maps/map%d.dat", player.level);
    infile.open(maploc);
    infile >> maploaded.rex >> maploaded.rey;
    for (int x = 0; x < MP_Height; x++)
    {
        infile >> maploaded.map[x];
    }
    infile.close();
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
    std::ifstream infile;
    infile.open("./data/save.dat");
    infile >> player.health >> player.attach >> player.level;
    for (int i = 0; i < 100; i++)
    {
        infile >> player.levelsave[i][0] >> player.levelsave[i][1] >> player.levelsave[i][2];
    }
    infile.close();
}

void ClearSrc()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 30});
    puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

void SetWindowSize(int width, int height)
{
    char setWinSize[64];
    sprintf(setWinSize, "mode con cols=%d lines=%d", width, height);
    system(setWinSize);
}

void Init()
{
    system("chcp 65001"); // UTF 8 mode
    setlocale(LC_ALL, "en_US.UTF-8");
    // system("color 0F");
    // setvbuf(stdout, NULL, _IONBF, 0);

    if (GameMode)
    {
        SetWindowSize(WinWidth, WinHeight + 1);
    }
    else
    {
        SetWindowSize(WinWidth, WinHeight);
    }

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;  // Width of each character in the font
    cfi.dwFontSize.Y = 16; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");                                // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi); // set font

    ClearSrc();

    _mkdir("./data");
    _mkdir("./data/maps");

    ReadSave();

    CheckSave();

    ReadMap();

    CheckMap();

    Save();

    SetConsoleTitleW(L"Game");
    puts("Preas any key to start!");
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

// End of Functions