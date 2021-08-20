void Shellsort(int a[], int len)  //希尔排序
{
    for (int x = len / 2; x > 0; x = x / 2)    //计算出每次的比较间隔
    {
        for (int y = x; y < len; y++)     //不同增量的插入排序
        {
            for (int z = y; (z - x) >= 0 && a[z - x] > a[z]; z -= x)
            {
                int tmp = a[z];
                a[z] = a[z - x];
                a[z - x] = tmp;
            }
        }
    }

}