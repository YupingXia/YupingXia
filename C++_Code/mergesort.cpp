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
            a[k] = aux[j - left];
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
void Test_Merge_sort(int* a, int len)
{

    Merge_sort(a, 0, len - 1);
}