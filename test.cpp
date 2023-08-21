#include <cwchar>
#include <windows.h>
#include <iostream>

#include <fstream>
#include <conio.h>
#include <locale.h>
#include <fstream>
#include <direct.h>
#include <math.h>
#include <stdarg.h>

using namespace std;
double abs_in_muti_dimension(int D, ...) // distance // dimension, x,y,z...
{
    va_list args;
    va_start(args, D);
    double tmp = 0.0;
    for (int i = 0; i < D; i++)
    {
        double cur = double(va_arg(args, double)); //
        tmp += cur * cur;
    }
    va_end(args);
    return sqrt(tmp);
}

int main(int argc, char const *argv[])
{
    cout << abs_in_muti_dimension(3, 1.0, 1.0, 1.0) << endl;
    return 0;
}
