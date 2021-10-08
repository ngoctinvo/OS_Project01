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
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

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
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

char *User2System(int virtAddr, int limit) {
    int i;
    int oneChar;
    char *kernelBuf = NULL;
    kernelBuf = new char[limit+1];
    if (kernelBuf==NULL) return kernelBuf;
    memset(kernelBuf,0,limit+1);
    for (i=0;i<limit;i++) {
        machine->ReadMem(virtAddr+i,1,&oneChar);
        kernelBuf[i]=(char)oneChar;
        if (oneChar==0) break;
    }
    return kernelBuf;
}

// transfer data from system to user
int System2User(int  virtAddr,int len,char *buffer) {
    if (len < 0) return -1;
    if (len == 0 ) return 0;
    int i=0;
    int oneChar=0;
    do {
        oneChar=(int)buffer[i];
        machine->WriteMem(virtAddr+i,1,oneChar);
        i++;
    } while(i < len && oneChar!=0);
    return i;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which) {
        case NoException:
            printf("Everything ok!\n");
            return;
        case SyscallException:
            switch (type) {
                case SC_Halt:
                    DEBUG('a', "Shutdown, initiated by user program.\n");
                    printf("Shutdown, initiated by user program.\n");
                    interrupt->Halt();
                    break;
                
                case SC_PrintNum: {
                    int num = machine->ReadRegister(4); //input number
                    char *out = new char[10]; //output string
                    memset(out,0,10);
                    if (num == 0) { // Special case 0
                        out[0]='0';
                        gSynchConsole->Write(out,1); // Print 0
                    }
                    else {
                        int leng = 0;
                        int sign = (num>0)?1:-1; // get sign
                        num = num * sign; // abs(num)
                        while (num > 0) { // abs(num) -> string (being reversed)
                            out[leng++]=(char)(num%10+'0');
                            num/=10;
                        }
                        if (sign == -1) out[leng++]='-'; // add minus if negative
                        char swap; // reverse string
                        for (int i = 0; i < leng / 2; i++) {
                            swap = out[i];
                            out[i]=out[leng-i-1];
                            out[leng-i-1]=swap;
                        }
                        gSynchConsole->Write(out,leng); // Print 'out'
                    }
                    delete[] out;
                    break;
                }
                
                case SC_ReadString: {
                    int virtAddr = machine->ReadRegister(4); //get directory of string
                    int leng = machine->ReadRegister(5); // get length
                    if (leng <= 0) { //error
                        interrupt->Halt();
                        return;
                    }
                    char *string = new char[leng+1]; // create string on system
                    int ok = gSynchConsole->Read(string, leng); // read on user
                    if (ok==-1) { //error
                        interrupt->Halt();
                        return;
                    }
                    if (ok==0) break; //user did not type
                    string[ok]='\0'; // add '\0' to the end
                    System2User(virtAddr, leng+1, string); // transfer to user
                    delete[] string;
                    break;
                }
                case SC_PrintString: {
                    int virtAddr = machine->ReadRegister(4); // get directory of string
                    char *string = User2System(virtAddr, 255); // get string from user
                    gSynchConsole->Write(string, 255); // print to screen
                    delete[] string;
                    break;
                }
           
                default:
                    printf("Unexpected user mode exception %d %d\n", which, type);
	            ASSERT(FALSE);
            }
            machine->registers[PrevPCReg]=machine->registers[PCReg];
            machine->registers[PCReg]=machine->registers[NextPCReg]; // Step to next PC
            machine->registers[NextPCReg]+=4;
            break;
        case PageFaultException:
            printf("No valid translation found\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        case ReadOnlyException:
            printf("Write attempted to page marked  \"read-only\"\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        case  BusErrorException:
            printf("Translation resulted in an invalid physical address\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        case AddressErrorException:
            printf("Unaligned reference or one that was beyond the end of the address space\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        case OverflowException:
            printf("Integer overflow in add or sub.\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        case IllegalInstrException:
            printf("Unimplemented or reserved instr.\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        case NumExceptionTypes:
            printf("NumExceptionTypes\n");
            ASSERT(FALSE);
            interrupt->Halt();
            break;
        default:
            printf("Unexpected user mode exception %d %d\n", which, type);
            ASSERT(FALSE);
    }
};
