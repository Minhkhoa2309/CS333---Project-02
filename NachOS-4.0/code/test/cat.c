#include "syscall.h"

int main() {
    char *fileName;
    int filelength;
    int length;
    int fileid;
    int len;
    int closefile;
    int lengthRead;
    
    fileid = -1;
    closefile = -1;
    lengthRead = 0;
    

    PrintString("Enter file name's length: ");
    length = ReadNum();
    PrintString("Enter file name: ");
    ReadString(fileName, length);


    fileid = Open(fileName);
    filelength = Seek(-1, fileid);
    if (fileid != -1) {

        Seek(0, fileid);
        
        while (filelength > 0) {
            // Maximun buffer for a read is 512 characters
            char buffer[512];
            buffer[511] = '\0';
            len = 0;
            if (filelength > 511) {
                Read(buffer, 511, fileid);
                filelength = filelength - 511;
                Seek(511, fileid);
            } else {
                lengthRead = Read(buffer, filelength, fileid);
                while (len <= filelength) {
                    len++;
                }
                buffer[len] = '\0';
                filelength = filelength - filelength;
            }
            PrintString("Buffer read: ");
            PrintString(buffer);
            PrintString("\n");
        }
        closefile = Close(fileid);
    } else {
        PrintString("Can't find file.\n");
    }

    

    Halt();
}
