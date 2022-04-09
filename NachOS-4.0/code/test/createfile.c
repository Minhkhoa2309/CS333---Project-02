#include "syscall.h"

int main() {
    char *fileName;
    int length,id;

    PrintString("Enter file name's length: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);

    if (Create(fileName) == 0) {
        PrintString("Created successfully!\n");
    } else
        PrintString("Create file failed\n");


    Halt();
}
