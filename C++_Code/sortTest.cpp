
//*********************冒泡排序法********************************//
/*
#author YupingXia
#vrsion v1.0
#include <iostream>
#include<stdio.h>
int main()
{
    int a[10] = { 4,35,45,234,765,23,65,23,43,567};
    for (int x = 0; x < 10; x++)
    {
        for (int y = x+1; y < 10; y++)
        {
            if (a[x] > a[y])
            {
                int tmp = a[y];
                a[y] = a[x];
                a[x] = tmp;
            }
        }
    }
    std::cout<< "hello ";
    for (int i = 0; i < 10; i++)
    {
        printf("%d,", a[i]);
    }
    printf("\n");
    system("pause");
}*/
//*********************冒泡排序法********************************//
//*********************插入排序法********************************//
/*#include<stdio.h>
#include<iostream>
#include<string.h>
void selectionsort(int *a, int len)
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
    selectionsort(a,len);
    showarrary(a,len);
}*/
//*********************插入排序法********************************//
//*********************希尔排序法********************************//

#include<stdio.h>
#include<iostream>
#include<string.h>
#include<ctime>
#include <cassert>
#include <string>
#include <sorttest.h>

void shellsort(int a[],int len)
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
void Shellsort(int a[], int len)
{
    for (int x = len / 2; x > 0; x = x / 2)    //计算出每次的比较间隔
    {
        for (int y = x; y < len; y++)
        {
            //int tmp = a[y];  //需要与前面进行比较的元素
            
            for (int z = y; (z - x) >= 0 && a[z - x] > a[z]; z -= x)
            {
                int tmp = a[z];
                a[z] = a[z - x];
                a[z - x] = tmp;
            }
           // a[z] = tmp;
        }
    }
}
void shellSort(int arr[], int n) {

    // 计算 increment sequence: 1, 4, 13, 40, 121, 364, 1093...
    int h = 1;
    while (h < n / 3)
        h = 3 * h + 1;

    while (h >= 1) {

        // h-sort the array
        for (int i = h; i < n; i++) {

            // 对 arr[i], arr[i-h], arr[i-2*h], arr[i-3*h]... 使用插入排序
            int e = arr[i];
            int j;
            for (j = i; j >= h && e < arr[j - h]; j -= h)
                arr[j] = arr[j - h];
            arr[j] = e;
        }

        h /= 3;
    }
}
void SelectionSort(int arr[], int n) {

    int left = 0, right = n - 1;
    while (left < right) {
        int minIndex = left;
        int maxIndex = right;

        // 在每一轮查找时, 要保证arr[minIndex] <= arr[maxIndex]
        if (arr[minIndex] > arr[maxIndex])
            swap(arr[minIndex], arr[maxIndex]);

        for (int i = left + 1; i < right; i++)
            if (arr[i] < arr[minIndex])
                minIndex = i;
            else if (arr[i] > arr[maxIndex])
                maxIndex = i;

        swap(arr[left], arr[minIndex]);
        swap(arr[right], arr[maxIndex]);

        left++;
        right--;
    }

    return;
}
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
void SelectionSort_Fast(int* a, int len)
{
    for (int x = 1; x < len; x++)
    {
        for (int y = x; y>0 && a[y]<a[y-1]; y--)
        {
            int tmp = a[y];
            a[y] = a[y - 1];
            a[y - 1] = tmp;
        }
    }
}
void insertionSort(int arr[], int n) {

    for (int i = 1; i < n; i++) {

        // 寻找元素arr[i]合适的插入位置
        // 写法1
//        for( int j = i ; j > 0 ; j-- )
//            if( arr[j] < arr[j-1] )
//                swap( arr[j] , arr[j-1] );
//            else
//                break;

        // 写法2
//        for( int j = i ; j > 0 && arr[j] < arr[j-1] ; j -- )
//            swap( arr[j] , arr[j-1] );

        // 写法3
        int e = arr[i];
        int j; // j保存元素e应该插入的位置
        for (j = i; j > 0 && arr[j - 1] > e; j--)
            arr[j] = arr[j - 1];
        arr[j] = e;
    }

    return;
}
void Merge_Sort(int* a, int mid, int left, int right)
{
    int* aux = new int[right - left + 1];  //分配一动态数组
    for (int i = left; i <= right; i++)    //将a[left]-a[right]赋值给新数组，从下表0开始
        aux[i - left] = a[i];
    int i = left, j = mid + 1;
    for (int k = left; k <= right; k++)
    {
        if (i > mid)
        {
            a[k] = aux[j-left];
            j++;
        }
        else if (j > right)
        {
            a[k] = aux[i - left];
            i++;
        }
        else if (aux[i - left] < aux[j - left])
        {
            a[k] = aux[i - left];
            i++;
        }
        else
        {
            a[k] = aux[j - left];
            j++;
        }
    }
    delete[] aux;
}

void Merge_sort(int* a, int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    Merge_sort(a, left, mid);
    Merge_sort(a, mid + 1, right);
    Merge_Sort(a, mid, left, right);
}
void Test_Merge_sort(int *a,int len)
{

    Merge_sort(a, 0, len - 1);
}
void swap( int i, int j)
{
    int tmp =i;
    i = j;
    j = tmp;
}
int partion(int* a, int left, int right)  //随机快速排序
{
    int j = left;
    int tmp = a[left];
    for (int i = left+1; i <= right;i++)
    {
        if (a[i] < tmp)
        {
            j++;
            int tmp1 = a[j];
            a[j ] = a[i];
            a[i ] = tmp1;
        }
    }
    a[left ] = a[j ];
    a[j ] = tmp;
    return j ;
}
int doublepartion(int* a, int left, int right)
{
    int begin = left+1;
    int end = right;
    int standsample = a[left];
    while (true)
    {
        while (end >= left + 1 && a[end] > standsample)
            end--;
        while (begin <= right && a[begin] < standsample)
            begin++;


        if (begin > end)
            break;
        int tmp = a[end];
        a[end] = a[begin];
        a[begin] = tmp;
        begin++, end--;
    }
    a[left] = a[end];
    a[end] = standsample;
    return end;
}
void threepartion(int *a,int left,int right)
{
    if (left >= right)
        return ;
    int tmp = a[left];
    int lt = left;
    int gt = right+1;
    int i = left + 1;
    while (i < gt)
    {
        if (a[i] < tmp)
        {
            int tmp1 = a[i];
            a[i] = a[lt+1];
            a[lt + 1] = tmp1;
            i++;
            lt++;
        }
        else if (a[i] > tmp)
        {
            int tmp2 = a[i];
            a[i] = a[gt-1];
            a[gt-1] = tmp2;
            gt--;
        }
        else
        {
            i++;
        }
    }
    a[left] = a[lt];
    a[lt] = tmp;
   
    threepartion(a, left, lt - 1);
    threepartion(a, gt, right);
}
void test_threepartion(int* a, int n)
{
    threepartion(a,0,n-1);
}


void quicksort(int* a, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int p = doublepartion(a, left, right);
    quicksort(a,left,p-1);
    quicksort(a, p + 1, right);
}
void Quicksort(int* a, int n)
{
    quicksort(a,0,n-1);
}
void showarrary(int* a, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d,",a[i]);
    }
    printf("\n");
}


int main(void)
{
    int len = 20000;
    int* a = SortTestHelper::generateRandomArray(len, 0, len);
    
    //showarrary(a, len);
    int aa[] = {9,5,4,7,89,456,89,45,4,78,45,4578,45,9,9,9,9,94,85,78,9,9,4,89};
    int n = sizeof(aa) / sizeof(aa[0]);
    //Quicksort(aa,n);
    test_threepartion(aa,n);
    //printf("partion:%d\n",p);
     showarrary(aa, n);
    SortTestHelper::testSort("quick Sort:", Quicksort, a,len);
    SortTestHelper::testSort("Shell Sort:", Shellsort, a, len); 
    SortTestHelper::testSort("test_threepartion Sort:", test_threepartion, a, len);
    system("pause");
}
//*********************希尔排序法********************************//

