#include<stdio.h>
#include<iostream>
#include<string.h>
void selectionsort(int* a, int len)
{
    for (int x = 1; x < len; x++)
    {
        for (int y = x; y > 0; y--)
        {
            if (a[y] < a[y - 1])
            {
                int tmp = a[y];
                a[y] = a[y - 1];
                a[y - 1] = tmp;


            }
        }
    }
}
void showarrary(int a[], int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\n");
    system("pause");
}
int main(void)
{
    int a[] = { 4,35,-2,0,-8,456,54,12,45,234,765,23,65,23,43,567 };
    int len = sizeof(a) / sizeof(a[0]);
    selectionsort(a, len);
    showarrary(a, len);
}