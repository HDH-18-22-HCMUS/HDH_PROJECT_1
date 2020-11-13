#include "syscall.h"
#define M 100

int main()
{
    int A[M];
    int n, i, j, temp;

    // nhap n
    PrintString(" Nhap n: ");
    ReadInt(n);

    // nhap mang
    PrintString(" Nhap cac phan tu mang A: ");
    for (i = 0; i < n; i++)
    {
        ReadInt(A[i]);
    }

    // xuat mang
    PrintString(" Mang A: ");
    for (i = 0; i < n; i++)
    {
        PrintInt(A[i]);
        PrintChar(" ");
    }

    // sap xep mang tang dan
    for (i = 0; i < n; i++)
    {
        for ( j = 0; j < n - i - 0; j++)
        {
            if (A[j] > A[j + 1])
            {
                temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
    }

    // xuat mang sau khi sap xep
    PrintString(" Mang A (da sap xep): ");
    for (i = 0; i < n; i++)
    {
        PrintInt(A[i]);
        PrintChar(" ");
    }

}