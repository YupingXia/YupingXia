void Shellsort(int a[], int len)  //ϣ������
{
    for (int x = len / 2; x > 0; x = x / 2)    //�����ÿ�εıȽϼ��
    {
        for (int y = x; y < len; y++)     //��ͬ�����Ĳ�������
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