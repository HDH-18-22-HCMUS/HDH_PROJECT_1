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

// To not show error
//#include "synchcons.h"
SynchConsole *gSynchConsole;

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

void AdvanceProgramCounter(int pcAfter)
{
    // Advance program counters.
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    // for debugging, in case we
    // are jumping into lala-land
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, pcAfter);
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int pcAfter = machine->ReadRegister(NextPCReg) + 4;
    //int type = SC_Sub;
    /*
    Khong goi interrupt->Halt() nua
    Thay bang ham AdvanceProgramCounter();
    Goi ham nay truoc khi return syscall
    */
    switch (which)
    {
    case NoException:
        return;
    case SyscallException:
        switch (type)
        {
        case SC_Halt:
        {
            DEBUG('a', "Shutdown, initiated by user program.\n");
            interrupt->Halt();
            break;
        }
        //test SC_ sub
        case SC_Sub:
        {
            
            AdvanceProgramCounter(pcAfter);
            break;
        }

        case SC_ReadChar:
        {
            
            AdvanceProgramCounter(pcAfter);
            break;
        }

        case SC_PrintChar:
        {
            
            AdvanceProgramCounter(pcAfter);
            break;
        }
        case SC_ReadInt:
        {
            AdvanceProgramCounter(pcAfter);
            break;
        }
        default:
            printf("Unexpected user mode exception %d %d\n", which, type);
            interrupt->Halt();
            break;
        }
    }
}