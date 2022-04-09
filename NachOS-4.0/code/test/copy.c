#include "syscall.h"

int main() {
    char *fileName;
    char buffer[256];
    
    int length;
    int len;
    int length2;
    int fileid1;
    int fileid2;
    int closefile;
    int lengthRead;
    int lengthWrite;
    
    len = 0;
    lengthWrite = -1;
    fileid1 = -1;
    fileid2 = -1; 
    closefile = -1;
    lengthRead = 0;
    buffer[255] = '\0';

    PrintString("Enter file name's length: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);
    

    fileid1 = Open(fileName);
    if (fileid1 != -1) {
        PrintString(fileName);
        PrintString(" opened successfully!\n");
        
        lengthRead = Read(buffer, 255, fileid1);

        while (buffer[len] != '\0') ++len;

        if (Create("copyfile.txt") == 0) {
            PrintString("Copy file created successfully!\n");
        } else {
            PrintString("Create copy file failed\n");
        }
        
        fileid2 = Open("copyfile.txt");

        if (fileid2 != -1) {
            lengthWrite = Write(buffer, len, fileid2);
        } else {
            PrintString("Can not open copy file\n");
        }

    }

    closefile = Close(fileid1);
    closefile = Close(fileid2);

    if (fileid1 == -1 || closefile == -1 || lengthRead == -1 || lengthWrite == -1) {
        PrintString("Program failed somewhere!\n");
    }

    
    Halt();
}
