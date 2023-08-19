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
#define CLOSEUNDERLINE "\033[0m" // abandon

short GameMode = 0;

const char block[64][16] = {"  ", "██", "◯ ", "★", "!!"};                                                                                                                                            //
const char event_bl_caps[64][16] = {"AA", "BB", "CC", "DD", "EE", "FF", "GG", "HH", "II", "JJ", "KK", "LL", "MM", "NN", "OO", "PP", "QQ", "RR", "SS", "TT", "UU", "VV", "WW", "XX", "YY", "ZZ"};     // I DONT EVEN KNOW WHAT IM DOING
const char event_bl_nonecaps[64][16] = {"aa", "bb", "cc", "dd", "ee", "ff", "gg", "hh", "ii", "jj", "kk", "ll", "mm", "nn", "oo", "pp", "qq", "rr", "ss", "tt", "uu", "vv", "ww", "xx", "yy", "zz"}; //

const int WinHeight = (MP_Height + 3);
const int WinWidth = (MP_Width * 2);

char vram[10000];

struct EVENT
{
    char id;
    short type;
    char act[1024];
};

struct PLAYER
{
    int health;
    int attach;
    int level;
    short levelsave[100][3]; // passed?    last x    last y
} player;

struct MAP
{
    char map[100][100]; //  ---Y
                        // |
                        // X
    int rex;
    int rey;
    EVENT event[64];
} maploaded;

void DialogEvent(short id);
void EventCheck(int x, int y);
void EditEvent(short idpos);
static inline int strcmp_asm(const char *cs, const char *ct);
void CheckCommand(char *command);
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
void DialogEvent(short id)
{
    ClearSrc();
    puts("────────────────────────────────────────────────Text────────────────────────────────────────────────");
    int textpos = 0;
    while (maploaded.event[id].act[textpos] != 0)
    {
        if (maploaded.event[id].act[textpos] == '\n')
        {
            while (!_kbhit())
            {
                // press any key to continue;
                // _getch();
            }
            _getch();
        }
        printf("%c", maploaded.event[id].act[textpos++]);
        Sleep(50);
    }
}

void EventCheck(short x, short y)
{
    char bl = maploaded.map[x][y];
    if (maploaded.event[bl - 'A'].id != 0) // event exist
    {
        switch (maploaded.event[bl - 'A'].type)
        {
        case 0: // dialog
        {
            DialogEvent(bl - 'A');
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void EditEvent(short idpos)
{
    memset(maploaded.event[idpos].act, 0, sizeof(maploaded.event[idpos].act));
    ClearSrc();
    puts("┌───────────────────────────────────────────────Text───────────────────────────────────────────────┐");
    puts("└─────────────────────────────Enter END on a single line to end editing────────────────────────────┘");
    char line[128];
    memset(line, 0, sizeof(line));
    while (strcmp_asm(line, "END") != 0)
    {
        gets(line);
        if (strcmp_asm(line, "END") == 0)
        {
            break;
        }
        strcat(maploaded.event[idpos].act, line);
        strcat(maploaded.event[idpos].act, "\n");
    }
    Save();
}

static inline int strcmp_asm(const char *cs, const char *ct) // fast string cmp
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

void CheckCommand(char *command)
{
    // cut command
    short comc = 0;
    char comv[10][64];
    char *token;
    token = strtok(command, " ");
    while (token != NULL)
    {
        strcpy(comv[comc++], token);

        token = strtok(NULL, " ");
    }

    // run command
    if (!strcmp_asm(comv[0], "event"))
    {
        if (!strcmp_asm(comv[1], "ins"))
        {
            char id = comv[2][0];
            if (id >= 'A' && id <= 'z' && maploaded.event[id - 'A'].id != 0)
            {
                maploaded.map[player.levelsave[player.level][1]][player.levelsave[player.level][2]] = id;
            }
        }
        else if (!strcmp_asm(comv[1], "edit"))
        {
            char id = comv[2][0];
            if (id >= 'A' && id <= 'z')
            {
                maploaded.event[id - 'A'].id = id;
                maploaded.event[id - 'A'].type = comv[3][0] - '0';
                EditEvent(id - 'A');
            }
        }
    }
    else if (!strcmp_asm(comv[0], "help")) //
    {
    }
    else
    {
        return;
    }
}

void Render()
{
    ClearSrc();
    memset(vram, 0, sizeof(vram));

    sprintf(vram, "HP: %d    AT: %d    LV: %d    Location: %d,%d\n", player.health, player.attach, player.level, player.levelsave[player.level][1], player.levelsave[player.level][2]);
    for (int x = 0; x < MP_Height; x++)
    {
        for (int y = 0; y < MP_Width; y++)
        {
            switch (GameMode)
            {
            case 0:
            {
                if (x == player.levelsave[player.level][1] && y == player.levelsave[player.level][2])
                {
                    strcat(vram, "U ");
                }
                else if (maploaded.map[x][y] > '9')
                {
                    strcat(vram, block[4]);
                }
                else
                {
                    strcat(vram, block[maploaded.map[x][y] - '0']);
                }
                break;
            }
            case 1:
            {
                if (maploaded.map[x][y] > '9' && maploaded.map[x][y] >= 'A' && maploaded.map[x][y] <= 'Z')
                {
                    strcat(vram, event_bl_caps[maploaded.map[x][y] - 'A']);
                }
                else if (maploaded.map[x][y] > '9' && maploaded.map[x][y] >= 'a' && maploaded.map[x][y] <= 'z')
                {
                    strcat(vram, event_bl_nonecaps[maploaded.map[x][y] - 'a']);
                }
                else
                {
                    strcat(vram, block[maploaded.map[x][y] - '0']);
                }
                break;
            }
            default:
            {
                break;
            }
            }
        }
        strcat(vram, "\n");
    }
    if (GameMode == 0)
    {
        strcat(vram, "WASD: Move    F: Interactive    <>: Change Level    Enter: Save    ESC: Save and Quit");
    }
    else if (GameMode == 1)
    {
        strcat(vram, "WASD: Move    <>: Change Level    Enter: Save Map    ESC: Save Map and Quit\n");
        strcat(vram, "0: Clear Block    1: Block    2: Set Res Point    3:Set End Point    /: Command");
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
    else if (maploaded.map[x][y] == '1' && GameMode == 0)
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
        case 102: // interactive
        {
            EventCheck(player.levelsave[player.level][1], player.levelsave[player.level][2]);
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
                CheckMap();
            }
            break;
        }
        case 46: // >
        {
            if (player.level + 1 <= 99)
            {
                player.level++;
                ReadMap();
                CheckMap();
            }
            break;
        }
        case 13: // Enter
        {
            Save();
            break;
        }
        case 47: // "/"
        {
            if (GameMode == 1)
            {

                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 33});
                char comv[128];
                gets(comv);
                CheckCommand(comv);
            }
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
                maploaded.map[maploaded.rex][maploaded.rey] = '0';
                maploaded.map[player.levelsave[player.level][1]][player.levelsave[player.level][2]] = '2';
                maploaded.rex = player.levelsave[player.level][1];
                maploaded.rey = player.levelsave[player.level][2];
            }
            break;
        }
        case 51: // 3
        {
            if (GameMode == 1)
            {
                maploaded.map[player.levelsave[player.level][1]][player.levelsave[player.level][2]] = '3';
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
        outfile << player.health << " "
                << player.attach << " "
                << player.level << " \n";
        for (int i = 0; i < 100; i++)
        {
            outfile << player.levelsave[i][0] << " " << player.levelsave[i][1] << " " << player.levelsave[i][2] << "\n";
        }
        outfile.close();
    }

    char maploc[64];
    sprintf(maploc, "./data/maps/map%d.dat", player.level);
    outfile.open(maploc, std::ios::out | std::ios::trunc);
    outfile << maploaded.rex << " "
            << maploaded.rey << " \n";
    for (int x = 0; x < MP_Height; x++)
    {
        for (int y = 0; y < MP_Width; y++)
        {
            outfile << maploaded.map[x][y];
        }
        outfile << "\n";
    }
    for (int i = 0; i < 64; i++)
    {
        if (maploaded.event[i].id == 0)
        {
            continue;
        }
        outfile << maploaded.event[i].id << ' ' << maploaded.event[i].type << ' ';
        int pos = 0;
        while (maploaded.event[i].act[pos] != 0)
        {
            outfile << maploaded.event[i].act[pos];
            pos++;
        }
        outfile << '\0' << '\n';
    }
    outfile.close();
}

void CheckMap()
{
    bool legalrespoint = 1;
    if (maploaded.map[maploaded.rex][maploaded.rey] != '2')
    {
        legalrespoint = 0;
    }
    for (int x = 0; x < MP_Height; x++)
    {
        for (int y = 0; y < MP_Width; y++)
        {
            if (legalrespoint == 0 && maploaded.map[x][y] == '2')
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
    memset(maploaded.map, 0, sizeof(maploaded.map));
    memset(maploaded.event, 0, sizeof(maploaded.event));
    maploaded.rex = 0;
    maploaded.rey = 0;
    std::ifstream infile;
    char maploc[64];
    sprintf(maploc, "./data/maps/map%d.dat", player.level);
    infile.open(maploc);
    infile >> maploaded.rex >> maploaded.rey;
    for (int x = 0; x < MP_Height; x++)
    {
        infile >> maploaded.map[x];
    }
    for (int i = 0; i < 64; i++)
    {
        infile >> maploaded.event[i].id >> maploaded.event[i].type;
        if (maploaded.event[i].id == 0)
        {
            break;
        }
        infile.get(); // ignore the space between
        int pos = 0;
        while (1)
        {
            char ch = infile.get();
            if (ch > '\0')
            {
                maploaded.event[i].act[pos] = ch;
                pos++;
            }
            else
            {
                break;
            }
        }
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
    setbuf(stdin, NULL);
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
    cfi.dwFontSize.X = 10; // Width of each character in the font
    cfi.dwFontSize.Y = 20; // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Courier New");                             // Choose your font
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