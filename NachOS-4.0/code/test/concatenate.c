#include "syscall.h"

int main() {
    int length1;
    int length2;
    int length;
    char buffer1[256];
    char buffer2[256];
    char buf[512];
    int fileid1;
    int fileid2;
    int fileid3;

    // PrintString("Enter file 1 name's length: ");
    // length = ReadNum();
    // PrintString("Enter file 1 name: ");
    // ReadString(fileName, length);
    // PrintString("Enter file 2 name's length: ");
    // length = ReadNum();
    // PrintString("Enter file 2 name: ");
    // ReadString(fileName, length);

    fileid1 = Open("abc");
    fileid2 = Open("def");
    length1 = 0;
    length2 = 0;
    length = 0;

    if (fileid1 != -1 && fileid2 != -1) {
        Read(buffer1, 255, fileid1);
        
        while(buffer1[length1] != '\0') {
            buf[length] = buffer1[length1];
            length++;
            length1++;
        }

        Read(buffer2, 255, fileid2);

        while(buffer2[length2] != '\0') {
            buf[length] = buffer2[length2];
            length++;
            length2++;
        }

        buf[length] = '\0';

        PrintString(buf);
        PrintString("\n");

        if (Create("Concatenate.txt") == 0) {
            PrintString("Concatenate file created successfully!\n");
        } else {
            PrintString("Create Concatenate file failed\n");
        }
        
        fileid3 = Open("Concatenate.txt");
        Write(buf, length, fileid3);

        Close(fileid1);
        Close(fileid2);
        Close(fileid3);
    }
    else {
        PrintString("Can not open file\n");
    }

    Halt();
}