#include "syscall.h"

int main() {
    char *fileName;
    char buffer[256];
    int length;
    int fileid;
    int closefile;
    int lengthRead;
    
    fileid = -1;
    closefile = -1;
    lengthRead = 0;
    buffer[255] = '\0';

    PrintString("Enter file name's length: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);


    fileid = Open(fileName);
    if (fileid != -1) {
        PrintString(fileName);
        PrintString(" opened successfully!\n");
        
        lengthRead = Read(buffer, 255, fileid);
        

        PrintString("Buffer read: ");
        PrintString(buffer);
        PrintString("\n");

        
    }

    if (fileid != -1) {
        closefile = Close(fileid);
    }

    if (closefile != -1) {
        PrintString(fileName);
        PrintString(" closed successfully!\n");
    }

    if (fileid == -1 || closefile == -1 || lengthRead == -1) {
        PrintString("Program failed somewhere!\n");
    }
    Halt();
}
