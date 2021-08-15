#include<stdio.h>
#include<iostream>
#include<string.h>
void shellsort(int a[], int len)
{
    for (int x = len / 2; x > 0; x = x / 2)    //计算出每次的比较间隔
    {
        for (int y = 0; y < x; y++)    //计算出需要进行插入排序的组数
        {
            for (int z = y; z < len; z += x)  //进行插入排序操作
            {
                for (int p = z; p > 0; p -= x)
                {
                    if (a[p] < a[p - x] && (p - x) >= 0)
                    {
                        int tmp = a[p];
                        a[p] = a[p - x];
                        a[p - x] = tmp;
                    }
                }
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
    int a[] = { 4,35,45,234,0,765,23,65,23,43,567,-12,41,67,35,45,51,41,56,-9,-5,0 };
    int len = sizeof(a) / sizeof(a[0]);    //判断数组长度
    shellsort(a, len);
    showarrary(a, len);
}