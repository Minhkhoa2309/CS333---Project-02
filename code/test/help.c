#include "syscall.h"

int main()
{
    PrintString("Team member:\n");
    PrintString("19125083\t Thai Ngoc Thanh Dat\n");
    PrintString("19125100\t Ta Ngoc Minh Khoa\n");
    PrintString("19125118\t Nguyen Hoang Son\n\n");
    PrintString("Running instruction for ASCII program and Bubble Sort program: \n ");

    PrintString("\t- ASCII: inside  NachOS-4.0/code, run build.linux/nachos -x test/ascii \n");

    PrintString("\t- Sort: inside  NachOS-4.0/code, run build.linux/nachos -x test/sort \n");
    PrintString("\t\t+ 1. Enter length of input array, (0 <= n <= 100)\n");
    PrintString("\t\t+ 2. Enter elements of input array\n");
    PrintString("\t\t+ 3. Enter the sort order (0: decrease, 1: increase)\n");

    Halt();
}
