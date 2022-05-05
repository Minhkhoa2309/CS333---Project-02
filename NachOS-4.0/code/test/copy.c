#include "syscall.h"

int main() {

    char *fileName;
    char *fileName2;
    
    int length;
    int length1;
    int len;
    int length2;
    int fileid1;
    int fileid2;
    int closefile;
    int lengthRead;
    
    len = 0;
    fileid1 = -1;
    fileid2 = -1; 
    closefile = -1;
    lengthRead = 0;
    // buffer[255] = '\0';

    PrintString("Enter copy file name's length: ");
    length = ReadNum();
    PrintString("Enter copy file name: ");
    ReadString(fileName, length);

    fileid1 = Open(fileName);
    length2 = Seek(-1, fileid1);


    PrintString("Enter destination file name's length: ");
    length1 = ReadNum();
    PrintString("Enter destination file name: ");
    ReadString(fileName2, length1);
    
    if (Create(fileName2) != 0) {
        PrintString("Create copy file failed\n");
    }
    fileid2 = Open(fileName2);
    
    if (fileid1 != -1 && fileid2 != -1) {

        Seek(0, fileid1);
        Seek(0, fileid2);

        while (length2 > 0) {
            // Maximun buffer for a read is 512 characters
            char buffer[512];
            buffer[511] = '\0';
            len = 0;
            if (length2 > 511) {
                Read(buffer, 511, fileid1);
                Write(buffer, 511, fileid2);
                length2 = length2 - 511;
                Seek(-1, fileid2);
                Seek(511, fileid1);
            } else {
                lengthRead = Read(buffer, length2, fileid1);
                while (len <= length2) {
                    len++;
                }
                buffer[len] = '\0';
                Write(buffer, len - 1, fileid2);
                length2 = length2 - length2;
            }
        }
        closefile = Close(fileid1);
        closefile = Close(fileid2);
    } else {
        PrintString("Can't open file.\n");
    }

    

    Halt();
}
