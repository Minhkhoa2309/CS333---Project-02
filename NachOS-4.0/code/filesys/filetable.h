#ifndef FILETABLE_H
#define FILETABLE_H
#include "openfile.h"
#include "sysdep.h"


/*

User program may require processing with mutiple files
In order to achieve such behavior we need a file table
which can store multiple files for processing

*/

class FileTable {
   private:
    OpenFile** openFile;

   public:
    FileTable() {
        openFile = new OpenFile*[10]; //  Max file : 10
        // fileOpenMode = new int[10]; //  Max file : 10
        // fileOpenMode[0] = 1;
        // fileOpenMode[1] = 2;
        
    }

    int Insert(char* fileName) {
        int freeIndex = -1;
        int fileDescriptor = -1;
        for (int i = 2; i < 10; i++) {
            if (openFile[i] == NULL) {
                freeIndex = i;
                break;
            }
        }
        if (freeIndex == -1) {
            return -1;
        }
        fileDescriptor = OpenForReadWrite(fileName, FALSE);
        if (fileDescriptor == -1) return -1;
        openFile[freeIndex] = new OpenFile(fileDescriptor);
        // fileOpenMode[freeIndex] = 0;
        return freeIndex;
    }

    int Remove(int index) {
        if (index < 2 || index >= 10) return -1;
        if (openFile[index]) {
            delete openFile[index];
            openFile[index] = NULL;
            return 0;
        }
        return -1;
    }

    int Read(char* buffer, int size, int index) {
        if (index >= 10) return -1;
        if (openFile[index] == NULL) return -1;
        int result = openFile[index]->Read(buffer, size);
        // if we cannot read enough bytes, we should return -2
        if (result != size) return -2;
        return result;
    }

    int Write(char* buffer, int size, int index) {
        if (index >= 10) return -1;
        if (openFile[index] == NULL)
            return -1;
        return openFile[index]->Write(buffer, size);
    }

    int Seek(int pos, int index) {
        if (index <= 1 || index >= 10) return -1;
        if (openFile[index] == NULL) return -1;
        // use seek(-1) to move to the end of file
        if (pos == -1) pos = openFile[index]->Length();
        if (pos < 0 || pos > openFile[index]->Length()) return -1;
        return openFile[index]->Seek(pos);
    }

    ~FileTable() {
        for (int i = 0; i < 10; i++) {
            if (openFile[i]) delete openFile[i];
        }
        delete[] openFile;
        // delete[] fileOpenMode;
    }
};

#endif