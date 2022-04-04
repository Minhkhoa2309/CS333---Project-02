#include "syscall.h"

int main()
{
    int result;

    result = RandomNum();

    PrintString("\n");

    Halt();
    /* not reached */
}