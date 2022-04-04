#include "syscall.h"

int main()
{
    char c = ReadChar();
    PrintChar(c);
    PrintChar('\n');
    Halt();
    /* not reached */
}
