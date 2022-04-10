/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"

// Maximum integer length
// For numIO
char inputNumBuffer[13];

bool SysCreate(char* fileName) {
    bool success;
    int length = strlen(fileName);
     //File name can't be empty
    if (length == 0) {
        success = false;
    }        //Not enough memory in system
    else if (fileName == NULL) {
        success = false;
    }        //File's name read successfully 
    else {
              //Error creating file
        if (!kernel->fileSystem->Create(fileName)) {
            success = false;
        } else {
            success = true;
        }
    }
    return success;
}


int SysOpen(char* fileName) {
    int id = kernel->fileSystem->OpenF(fileName);
    return id;
}

int SysClose(int id) { 
  return kernel->fileSystem->Close(id); 
}

int SysRead(char* buffer, int size, int id) {
  DEBUG(dbgSys, "File id: " << id << "\n");
  int result = kernel->fileSystem->Read(buffer, size, id);
  DEBUG(dbgSys, "Read return: " << result << "\n");

  DEBUG(dbgSys, "Buffer return: " << buffer << "\n");

  return result;
}

int SysWrite(char* buffer, int size, int id) {
    return kernel->fileSystem->Write(buffer, size, id);
}

void SysPrintNum(int num)
{

  if (num == 0)
    return kernel->synchConsoleOut->PutChar('0');

  if (num == -2147483648)
  {
    kernel->synchConsoleOut->PutChar('-');
    for (int i = 0; i < 10; ++i)
      kernel->synchConsoleOut->PutChar("2147483648"[i]);
    return;
  }

  if (num < 0)
  {
    kernel->synchConsoleOut->PutChar('-');
    num = -num;
  }
  int n = 0;
  while (num)
  {
    inputNumBuffer[n++] = num % 10;
    num /= 10;
  }
  for (int i = n - 1; i >= 0; --i)
    kernel->synchConsoleOut->PutChar(inputNumBuffer[i] + '0');
}

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

char SysReadChar()
{
  return kernel->synchConsoleIn->GetChar();
}

void SysPrintChar(char character)
{
  kernel->synchConsoleOut->PutChar(character);
}

int SysRandomNum()
{
  srand(time(0));
  return rand() % 2147483647 + 1;
}

void SysReadString(char *&buffer, int length)
{
  buffer = new char[length + 1];
  for (int i = 0; i < length; i++)
  {
    buffer[i] = kernel->synchConsoleIn->GetChar();
  }
  buffer[length] = '\0';
}

void SysPrintString(char *buffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    kernel->synchConsoleOut->PutChar(buffer[i]);
  }
}

/*Print printable ascii characters*/
void SysPrintASCII()
{
  int count = 0;
  for (int i = 32; i <= 126; i++)
  {
    SysPrintChar((char)i);
    SysPrintChar(' ');
    count++;
    if (count % 5 == 0)
    {
      SysPrintChar('\n');
    }
  }
  SysPrintChar('\n');
}

/**
 * Return true of the interger equals to the
 * interger stored in the string
 **/

bool compareNumAndString(int integer, const char *s)
{
  if (integer == 0)
    return strcmp(s, "0") == 0;

  int len = strlen(s);

  if (integer < 0 && s[0] != '-')
    return false;

  if (integer < 0)
    s++, --len, integer = -integer;

  while (integer > 0)
  {
    int digit = integer % 10;

    if (s[len - 1] - '0' != digit)
      return false;

    --len;
    integer /= 10;
  }

  return len == 0;
}

char isBlank(char c)
{
  return c == ((char)10) || c == ((char)13) || c == ((char)9) || c == ((char)' ');
}

void TilBlank()
{
  /**
   * Read and store characters into inputNumBuffer
   * will exit if encouter any non-numeric characters.
   * It will read at most 12 character
   **/

  // Using memset to avoid memory leaks
  memset(inputNumBuffer, 0, sizeof(inputNumBuffer));
  char c = kernel->synchConsoleIn->GetChar();

  if (c == EOF)
  {
    DEBUG(dbgSys, "Unexpected end of file - number expected");
    return;
  }

  if (isBlank(c))
  {
    DEBUG(dbgSys, "Unexpected white-space - number expected");
    c = kernel->synchConsoleIn->GetChar();
  }

  int n = 0;

  while (!(isBlank(c) || c == EOF))
  {
    inputNumBuffer[n++] = c;
    if (n > 11)
    {
      DEBUG(dbgSys, "Number is too long");
      return;
    }
    c = kernel->synchConsoleIn->GetChar();
  }
}

int SysReadNum()
{
  int len = strlen(inputNumBuffer);

  // Read nothing -> return 0
  if (len == 0)
    return 0;

  bool negative = (inputNumBuffer[0] == '-');
  int zero_lead = 0;
  bool is_leading = true;

  // Resulting integer
  int num = 0;

  // Start at index 1 if number is negative
  for (int i = negative; i < len; ++i)
  {
    char c = inputNumBuffer[i];

    if (c == '0' && is_leading)
      ++zero_lead;
    else
      is_leading = false;

    if (c < '0' || c > '9')
    {
      DEBUG(dbgSys, "Non numeric input " << inputNumBuffer << "\n");
      return 0;
    }

    num = num * 10 + (c - '0');
  }

  // Check for leading digits: 00, 01 or -0
  if (zero_lead > 1 || (zero_lead && (num || negative)))
  {
    DEBUG(dbgSys, "Non numeric input " << inputNumBuffer << "\n");
    return 0;
  }

  if (negative)
    num = num * -1;

  /* Check if buffer is correctly converted, avoiding edge cases where exceed maximum int32 value */
  if (compareNumAndString(num, inputNumBuffer))
    return num;
  else
    DEBUG(dbgSys, "Expected int32 number but " << inputNumBuffer << " found");

  return 0;
}

int SysSeek(int position, int id) {
    if (id <= 1) {
        DEBUG(dbgSys, "\nCan not seek file");
        return -1;
    }
    return kernel->fileSystem->Seek(position, id);
}

bool SysRemove(char* fileName){
      bool success;
    int length = strlen(fileName);
     //File name can't be empty
    if (length == 0) {
        success = false;
    }        //Not enough memory in system
    else if (fileName == NULL) {
        success = false;
    }        //File's name read successfully 
    else {
              //Error creating file
        if (!kernel->fileSystem->Remove(fileName)) {
            success = false;
        } else {
            success = true;
        }
    }
    return success;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
