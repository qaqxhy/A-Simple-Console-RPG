# A-Simple-Console-RPG
Windows system only!  
I suggest you download the whole `Binary` folder, its has the tutorial map I created.  
Compile Command:  
`g++ C:\Users\qaqxhy\CPP\Console-Game\Console-Game-C.cpp -o C:\Users\qaqxhy\CPP\Console-Game\Binary\BIN.exe -O3 -m64 -Wall -static-libgcc -fexec-charset=utf-8 -D__USE_MINGW_ANSI_STDIO`  
# 更新记录
    - 8/22/23: 见B站: `https://www.bilibili.com/video/BV1vP411p7fC/`
    - 8/21/23: 修复分数系统
    - 8/21/23: 修复教程关卡
    - 8/21/23: 添加CmdHelper文件, 用于help指令
    - 8/21/23: 添加指令reset-target重置目标
    - 8/21/23: 修改跳过对话
    - 8/19/23: 添加对话跳过及退出功能
    - 8/19/23: 把文字(对话)事件及存档完成
    - 8/19/23: 把创造模式下的事件渲染写完了

# 记录写代码遇到的问题  
## 输入  
1. 无堵塞键盘输入  
    - 循环检测`_kbhit()`函数, 为`true`时`_getch()`函数获取键盘按键  
## 输出(渲染 ?)  
1. `cout`太慢了, 而且取消流同步`_getch()`就会出问题  
2. `printf()`仍然太慢, 所以改用`sprintf()`到一个**中间字符串**并用`puts()`输出, 在代码中为变量`vram`  
    - **注意: `puts`会多输出一个换行, 所以初始化窗口时行数要加一**
3. 清屏时`system("cls");`会闪一下, 所以改用`puts`很多换行把以前的输出"顶"上去并且把光标移到`0,0`位置
    - 函数`SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {x, y});` 将光标移到`x,y`位置, 其中`x`和`y`为**short**型
4. **UTF-8输出**
    - `system("chcp 65001");` 将控制台设置为`UTF-8`模式
    - `setlocale(LC_ALL, "en_US.UTF-8");` 也是设置`UTF-8`模式
5. **控制台显示**
    - `system("mode con cols=11 lines=114514");` 将控制台设置为11宽114514高(个字符)
    - **设置字体, 不设置的话字符渲染有问题**  
    ```
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;  // Width of each character in the font
        cfi.dwFontSize.Y = 16; // Height
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        std::wcscpy(cfi.FaceName, L"Consolas");                                // Choose your font
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi); // set font
    ```
    - 设置窗口标题`SetConsoleTitleW(L"");`
## 文件读写  
1. `ofstream`和`ifstream`打开相对路径时, 不会主动创建目录, 但目录存在时会主动创建文件  
    - `_mkdir()`函数创建文件夹