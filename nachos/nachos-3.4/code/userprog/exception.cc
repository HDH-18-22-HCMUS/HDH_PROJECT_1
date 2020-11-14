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
#define MAX_LENGTH 255
// To not show error
//#include "synchcons.h"
//SynchConsole *gSynchConsole=new SynchConsole();

#include "machine.h"
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
// And don't forget to incremenSt the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void AdvanceProgramCounter()
{
    // Advance program counters.
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    //machine->registers[PrevPCReg] = machine->registers[PCReg];
    // for debugging, in case we
    // are jumping into lala-land
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    //machine->registers[PCReg] = machine->registers[NextPCReg];
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
    //machine->registers[NextPCReg] += 4;
}

/*
Copy buffer from userspace to kernelspace

Arguments:
int virtualAddress: Userspace address
int bufferSize: maximum size of buffer

Output:
buffer
*/
char *User2System(int virtualAddress, int bufferSize)
{
    int chr;
    char *kernelBuffer = NULL;
    kernelBuffer = new char[bufferSize + 1];

    if (kernelBuffer == NULL)
    {
        return kernelBuffer;
    }

    memset(kernelBuffer, 0, bufferSize + 1);

    for (int i = 0; i < bufferSize; i++)
    {
        machine->ReadMem(virtualAddress + i, 1, &chr);
        kernelBuffer[i] = (char)chr;
        if (chr == 0)
            break;
    }
    return kernelBuffer;
}

/*
Copy buffer from kernelspace to userspace

Arguments:
int virtualAddress: Userspace address
int bufferSize: maximum size of buffer
char* buffer: pointer to userspace buffer

Output:
int counter: number of bytes copied
*/

int System2User(int virtualAddress, int bufferSize, char *buffer)
{
    if (bufferSize < 0)
    {
        return -1;
    }
    if (bufferSize == 0)
    {
        return 0;
    }

    int counter = 0;
    int chr = 0;

    do
    {
        chr = (int)buffer[counter];
        machine->WriteMem(virtualAddress + counter, 1, chr);
        counter++;
    } while (counter < bufferSize && chr != 0);

    return counter;
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    //int type = SC_Sub;
    /*
    Khong goi interrupt->Halt() nua
    Thay bang ham AdvanceProgramCounter();
    Goi ham nay truoc khi return syscall
    */
    switch (which)
    {
    case SyscallException:
    {
        SynchConsole *gSynchConsole = new SynchConsole();
        switch (type)
        {
        case SC_Halt:
        {
            DEBUG('a', "Shutdown, initiated by user program.\n");
            interrupt->Halt();
            break;
            //test SC_ sub
        }
        case SC_Sub:
        {
            int op1 = machine->ReadRegister(4);
            int op2 = machine->ReadRegister(5);
            int result = op1 - op2;
            machine->WriteRegister(2, result);
            //DEBUG('a', "123dfhh.\n");
            break;
        }

        case SC_ReadChar:
        {
            int sz = 0;
            char *buf = new char[MAX_LENGTH];
            sz = gSynchConsole->Read(buf, MAX_LENGTH);
            machine->WriteRegister(2, buf[0]);
            delete[] buf;
            break;
        }

        case SC_PrintChar:
        {
            char ch = (char)machine->ReadRegister(4);
            gSynchConsole->Write(&ch, 1);
            break;
        }

        case SC_ReadInt:
        {
            int nByte = 11;
            char *str = new char[nByte];

            int len = gSynchConsole->Read(str, nByte); // do dai ky tu chuoi nhap.

           bool neg = false;
           bool isNum = true;
           int result = 0; //Bien ket qua.
           int check = 0; 
           int i = 0;

            // Nhan dien so nguyen am
           if (str[0] == '-') 
           {
               neg = true;
               i = 1;
               check = 1;
           }

            //Kiem tra chuoi nhap co phai la so nguyen hay khong
           for (; check < len; check++) 
           {
               if (str[check] < '0' || str[check] > '9') 
               {
                   isNum = false;
                   break;
               }
           }

           if (isNum) 
           {
               // Chuyen doi chuoi nhap thanh so.
                int pow = 1;
                for (; i < len; i++)
                {
                    result = result + (str[i] - '0') * pow; // Them ky tu vao bien ket qua.
                    pow = pow * 10;
                }
           }

            if (neg)
            {
                result = - result;
            }

            machine->WriteRegister(2, result);

            delete[] str;
            break;
        }

        case SC_PrintInt:
        {
            int num = machine->ReadRegister(4);
            char *str = new char[11]; // Dãy chữ số của num
            str [10] = '\n';          // Ky tu xuong hang
            int nByte = 0;            // Số chữ số trong num.
            
            //Kiem tra so am
            if (num < 0) 
            {
                char neg = '-';
                gSynchConsole->Write(&neg, 1); // In dau '-'
                num = - num; // doi num thanh so duong
            }

            int i = 10; 
            int p = 10;

            //Gan tung chu so cua num vao str
            do 
            {
                i--;
                nByte++;
                str[i] = num % p + '0';
                num = num / p;
            }
            while (num > 0);

            //In str bat dau tu str[i]
            gSynchConsole->Write(str + i, nByte + 1);
            
            delete[] str;
            break;
        }

        case SC_PrintString:
        {
            int i = 0;
            char *string = new char[MAX_LENGTH];
            string = User2System(machine->ReadRegister(4), MAX_LENGTH + 1);
            while (string[i] != 0 && string[i] != '\n')
            {
                gSynchConsole->Write(string + i, 1);
                i++;
            }

            gSynchConsole->Write(string + i, 1);
            delete[] string;
            break;
        }

        case SC_ReadString:
        {
            char *string = new char[MAX_LENGTH];
            if (string == 0) // out of save space
            {
                delete[] string;
                break;
            }

            int virtualAddress = machine->ReadRegister(4);
            int length = machine->ReadRegister(5);

            int bufferSize = gSynchConsole->Read(string, length);
            System2User(virtualAddress, bufferSize, string);
            delete[] string;
            break;
        }
        default:
        {
            printf("Unexpected user mode exception %d %d\n", which, type);
            interrupt->Halt();
            break;
        }
        }
        delete gSynchConsole;
        AdvanceProgramCounter();
        break;
    }
    case NoException:
        return;
    }
}
