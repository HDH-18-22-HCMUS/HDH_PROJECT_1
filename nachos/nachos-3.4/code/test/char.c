#include "syscall.h"
#include "copyright.h"
int main()
{
	char a[10];
	int i;
	for (i = 0; i < 5; i++)
	{
		a[i] = ReadChar();
	}

	for (i = 0; i < 5; i++)
	{
		PrintChar(a[i]);
		PrintChar(' ');
	}

	return 0;
}