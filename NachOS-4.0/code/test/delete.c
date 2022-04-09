#include "syscall.h"

int main() {
    char *fileName;
    int length;
    PrintString("Enter file name's length: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);

    if (Remove(fileName) != 1) {
        PrintString("Remove successfully\n");
    }
    else {
        PrintString("Remove unsuccessfully\n");
    }
    Halt();
}