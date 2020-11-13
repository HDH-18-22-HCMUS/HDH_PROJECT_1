#include "syscall.h"

int main()
{
    int count;
    for (count = 1; count < 128; count++)
    {
        PrintInt(count);
        //PrintString(" - ");
        PrintChar((char)count);
    }
}