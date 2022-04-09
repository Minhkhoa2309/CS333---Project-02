// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "synchconsole.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------
void PCIncrement()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

void Sys2User(int address, int length, char *buffer)
{
	for (int i = 0; i < length; i++)
	{
		kernel->machine->WriteMem(address + i, 1, (int)buffer[i]);
	}

	kernel->machine->WriteMem(address + length, 1, (int)'\0');
}

void User2Sys(int address, int *length, char *&buffer)
{
	int len = 0, ch;
	while (ch != '\0')
	{
		kernel->machine->ReadMem(address + len, 1, &ch);
		len++;
	}

	buffer = new char[len];

	for (int i = 0; i < len; ++i)
	{
		kernel->machine->ReadMem(address + i, 1, &ch);
		buffer[i] = (unsigned char)ch;
	}

	*length = len;
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);
	int address;
	int length;
	char *str;
	char *buffer;
	char charResult;
	int result;
	int fileid;
	int position;
	int l;

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			PCIncrement();
			return;

			ASSERTNOTREACHED();

			break;

		case SC_PrintNum:
			// DEBUG(dbgSys, "Print a integer number.\n");

			result = kernel->machine->ReadRegister(4);
			// DEBUG(dbgSys, "Number " << result << "\n");
			SysPrintNum(result);
			PCIncrement();
			return;
			ASSERTNOTREACHED();
			break;
		case SC_ReadChar:
			DEBUG(dbgSys, "Read a character.\n");

			charResult = SysReadChar();
			kernel->machine->WriteRegister(2, (char)charResult);
			PCIncrement();
			return;
			ASSERTNOTREACHED();
			break;

		case SC_RandomNum:
			DEBUG(dbgSys, "Generating a random number.\n");

			int random;
			random = SysRandomNum();

			DEBUG(dbgSys, "RandomNum returning with " << random << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)random);
			SysPrintNum(random);
			/* Modify return point */
			PCIncrement();
			return;
			ASSERTNOTREACHED();
			break;
		case SC_PrintChar:
			DEBUG(dbgSys, "Print character.\n");
			charResult = kernel->machine->ReadRegister(4);
			kernel->synchConsoleOut->PutChar((char)charResult);
			PCIncrement();
			return;
			ASSERTNOTREACHED();
			break;
		case SC_ReadNum:
			DEBUG(dbgSys, "Read Number.\n");
			TilBlank();
			result = SysReadNum();
			kernel->machine->WriteRegister(2, result);
			PCIncrement();
			return;
			ASSERTNOTREACHED();
			break;
		case SC_ReadString:
			// DEBUG(dbgSys, "Reading string at " << kernel->machine->ReadRegister(4) << " with a length of " << kernel->machine->ReadRegister(5) << "\n");

			/* Process Systemcall*/
			address = (int)kernel->machine->ReadRegister(4);
			length = (int)kernel->machine->ReadRegister(5);
			str = NULL;

			SysReadString(str, length);

			Sys2User(address, length, str);
			// DEBUG(dbgSys, "Written from kernel space to user space\n");

			if (str != NULL)
				delete[] str;

			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;

		case SC_PrintString:
			// DEBUG(dbgSys, "Printing string at " << kernel->machine->ReadRegister(4) << "\n");

			/* Process Systemcall*/
			address = (int)kernel->machine->ReadRegister(4);
			str = NULL;

			User2Sys(address, &length, str);
			// DEBUG(dbgSys, "Written from user space to kernel space\n");
			
			SysPrintString(str, length);
			if (str != NULL)
				delete[] str;
			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_PrintASCII:
			DEBUG(dbgSys, "Print ASCII.\n");
			SysPrintASCII();
			PCIncrement();
			return;
			ASSERTNOTREACHED();
			break;
		case SC_Create:
			DEBUG(dbgSys, "Create File.\n");
			address = (int)kernel->machine->ReadRegister(4);
			str = NULL;

    		User2Sys(address,&length,str);

			if (SysCreate(str))
				kernel->machine->WriteRegister(2, 0);
			else
				kernel->machine->WriteRegister(2, -1);

			if (str != NULL)
				delete[] str;
			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_Open:
			DEBUG(dbgSys, "Open File.\n");
			address = (int)kernel->machine->ReadRegister(4); 
			str = NULL;

			User2Sys(address,&length,str);
			DEBUG(dbgSys, "Written from user space to kernel space\n");

			result = SysOpen(str);
			kernel->machine->WriteRegister(2, result);

			if (str != NULL)
				delete[] str;
			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_Close:
			DEBUG(dbgSys, "Close File.\n");
			fileid = (int)kernel->machine->ReadRegister(4);

			result = SysClose(fileid);
    		kernel->machine->WriteRegister(2, result);

			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;

		case SC_Read: 
			DEBUG(dbgSys, "Read File.\n");
			address = (int)kernel->machine->ReadRegister(4);
    		length = (int)kernel->machine->ReadRegister(5);
			fileid = (int)kernel->machine->ReadRegister(6);
			str = NULL;

    		User2Sys(address, &l, str);
			DEBUG(dbgSys, "Written from user space to kernel space\n");
    		
			result = SysRead(str, length, fileid);

			Sys2User(address, 100, str);
			DEBUG(dbgSys, "Written from kernel space to user space\n");

    		kernel->machine->WriteRegister(2, result);

			
			if (str != NULL)
				delete[] str;

			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;
		
		case SC_Write:
			DEBUG(dbgSys, "Write File.\n");
			address = (int)kernel->machine->ReadRegister(4);
    		length = (int)kernel->machine->ReadRegister(5);
			fileid = (int)kernel->machine->ReadRegister(6);
			str = NULL;

    		User2Sys(address, &l, str);

			l -= 1;

			DEBUG(dbgSys, "Written from user space to kernel space\n");

			DEBUG(dbgSys, "Writting string " << str << " to file\n");

			DEBUG(dbgSys, "Writting with length " << l << " to file\n");

			result = SysWrite(str, l, fileid);
    		kernel->machine->WriteRegister(2, result);

    		Sys2User(address, l, str);
			DEBUG(dbgSys, "Written from kernel space to user space\n");

			if (str != NULL)
				delete[] str;

			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;

		case SC_Seek:
			position = (int)kernel->machine->ReadRegister(4);
    		fileid = (int)kernel->machine->ReadRegister(5);

    		kernel->machine->WriteRegister(2, SysSeek(position, fileid));

    		PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_Remove:
			DEBUG(dbgSys, "Create File.\n");
			address = (int)kernel->machine->ReadRegister(4);
			str = NULL;

    		User2Sys(address,&length,str);

			if (SysRemove(str))
				kernel->machine->WriteRegister(2, 0);
			else
				kernel->machine->WriteRegister(2, -1);

			if (str != NULL)
				delete[] str;
			PCIncrement();
			return;

			ASSERTNOTREACHED();
			break;
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	case PageFaultException:
		DEBUG(dbgAddr, "No valid translation found");
		printf("No valid translation found");
		SysHalt();
		break;
	case ReadOnlyException:
		DEBUG(dbgAddr, "Cannot write to the readonly page");
		printf("Cannot write to the readonly page");
		SysHalt();
		break;
	case BusErrorException:
		DEBUG(dbgAddr, "Translation resulted in an invalid physical address");
		printf("Translation resulted in an invalid physical address");
		SysHalt();
		break;
	case AddressErrorException:
		DEBUG(dbgAddr, "Unaligned reference or one that was beyond the end of the address space");
		printf("Unaligned reference or one that was beyond the end of the address space");
		SysHalt();
		break;
	case OverflowException:
		DEBUG(dbgAddr, "Integer overflow in add or sub");
		printf("Integer overflow in add or sub");
		SysHalt();
		break;
	case IllegalInstrException:
		DEBUG(dbgAddr, "Unimplemented or reserved instr.");
		printf("Unimplemented or reserved instr.");
		SysHalt();
		break;
	case NumExceptionTypes:
		DEBUG(dbgAddr, "Using wrong type");
		printf("Using wrong type");
		SysHalt();
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
