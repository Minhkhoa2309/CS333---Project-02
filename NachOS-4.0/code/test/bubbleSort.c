#include "syscall.h"
#define size (100)

void bubbleSort(int order, int *a, int n);

int main()
{
    int a[size + 1], n, i, order;

    PrintString("Enter number of element: ");
    n = ReadNum();

    for (i = 0; i < n; i++)
    {
        PrintString("Please enter a[");
        PrintNum(i);
        PrintString("]: ");
        a[i] = ReadNum();
    }

    PrintString("Enter 1 for ascending sort, 0 for descending sort: ");
    order = ReadNum();

    bubbleSort(order, a, n);

    PrintString("\nSorted array: ");

    for (i = 0; i < n; i++)
    {
        PrintNum(a[i]);
        PrintString(" ");
    }
    PrintString("\n");
    Halt();
}

void bubbleSort(int order, int *a, int n)
{
    int temp = 0, i, j;
    // order = 0: ascending order
    // order = 1: decending order
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            if (order >= 1)
            {
                if (a[j] > a[j + 1])
                {
                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            }
            else if (order <= 0)
            {
                if (a[j] < a[j + 1])
                {
                    temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                }
            }
        }
    }
}