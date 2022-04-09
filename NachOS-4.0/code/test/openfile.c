#include "syscall.h"

int main() {
    char *fileName;
    int length, id;
    int i;
    PrintString("Enter file name's length: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);
        if ((id = Open(fileName)) != -1) {
            PrintString(" Opened successfully!\n");
            PrintString("Id: ");
            PrintNum(id);
            PrintString("\n");
            Close(id);
        } else
            PrintString("Open file failed\n");
    Halt();
}
