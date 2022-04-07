#include "syscall.h"

int main() {
    char fileName[256];
    int length;

    PrintString("Enter length of create file name: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);

    if (CreateFile(fileName) == 0) {
        PrintString("Created successfully!\n");
    } else
        PrintString("Create file failed\n");
    Halt();
}
