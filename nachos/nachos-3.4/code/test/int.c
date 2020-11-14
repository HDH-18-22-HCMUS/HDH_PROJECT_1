#include "syscall.h"
#include "copyright.h"

int A[10];

int main()
{
    /*
	int a;
    PrintString("Nhap a: ");
    a = ReadInt();
    PrintString("=> a = ");
    PrintInt(a);
    */
   int n = 4;
   int i;
   for (i = 0; i < n; i++)
   {
       A[i] = ReadInt();
   }
   PrintString("\n");

    for (i = 0; i < n; i++)
    {
       PrintInt(A[i]);
       PrintChar(' ');
    }
    PrintString("\n");

	return 0;
}



















