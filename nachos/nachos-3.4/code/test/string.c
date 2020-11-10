#include "syscall.h"
#include "copyright.h"
int main()
{
	char str[100];
	ReadString(str, 10);
	PrintString(str);
	return 0;
}