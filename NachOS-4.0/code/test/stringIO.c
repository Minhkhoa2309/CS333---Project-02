
#include "syscall.h"

int main()
{
    char *str;
    int length;
    PrintString("Enter length of input string: ");
    length = ReadNum();
    PrintString("Enter input string: ");
    ReadString(str, length);
    // Result
    PrintString("Result string: ");
    PrintString(str);
    PrintString("\n");
    Halt();
    /* not reached */
}