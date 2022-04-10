#include "syscall.h"

int main() {
    int length1;
    int length2;
    int length;
    int lengthRead;
    int len;
    char *fileName1;
    char *fileName2;
    
    int fileid1;
    int fileid2;
    int fileid3;

    PrintString("Enter file 1 name's length: ");
    length = ReadNum();
    PrintString("Enter file 1 name: ");
    ReadString(fileName1, length);
    

    fileid1 = Open(fileName1);
    length1 = Seek(-1, fileid1);
    
    PrintString("Enter file 2 name's length: ");
    length = ReadNum();
    PrintString("Enter file 2 name: ");
    ReadString(fileName2, length);

    fileid2 = Open(fileName2);
    length2 = Seek(-1, fileid2);

    if (fileid1 != -1 && fileid2 != -1) {
        Seek(0, fileid1);
        Seek(0, fileid2);

        if (Create("Concatenate-result.txt") != 0) 
            PrintString("Create concatenate file failed!\n");

        fileid3 = Open("Concatenate-result.txt");

        while (length1 > 0) {
            // Maximun buffer for a read is 512 characters
            char buffer[512];
            buffer[511] = '\0';
            len = 0;
            if (length1 > 511) {
                Read(buffer, 511, fileid1);
                Write(buffer, 511, fileid3);
                length1 = length1 - 511;
                Seek(-1, fileid3);
                Seek(511, fileid1);
            } else {
                lengthRead = Read(buffer, length1, fileid1);
                while (len <= length1) {
                    len++;
                }
                buffer[len] = '\0';
                Write(buffer, len - 1, fileid3);
                length1 = length1 - length1;
            }
        }
        
        while (length2 > 0) {
            // Maximun buffer for a read is 512 characters
            char buffer[512];
            buffer[511] = '\0';
            len = 0;
            if (length2 > 511) {
                Read(buffer, 511, fileid2);
                Write(buffer, 511, fileid3);
                length2 = length2 - 511;
                Seek(-1, fileid3);
                Seek(511, fileid2);
            } else {
                lengthRead = Read(buffer, length2, fileid2);
                while (len <= length2) {
                    len++;
                }
                buffer[len] = '\0';
                Write(buffer, len - 1, fileid3);
                length2 = length2 - length2;
            }
        }
        
        Close(fileid1);
        Close(fileid2);
        Close(fileid3);
    }
    else {
        PrintString("Can not open file\n");
    }

    Halt();
}