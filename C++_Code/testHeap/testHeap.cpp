// testHeap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>
#include <cassert>
#include <algorithm>
#include <string>
#include <ctime>
#include <cmath>
#include <sorttest.h>
using namespace std;
//*堆的类定义
class MyClass
{
public:
	MyClass(int capacity);
	~MyClass();
	int size()
	{
		return count;
	}
	bool isEmpty()
	{
		return count == 0;
	}

	void insert(int item)
	{
		assert(count + 1 <= capacity);
		Data[count + 1] = item;
		shiftUp(count+1);
        count++;
	}
    void shiftDown(int k)
    {
        while (2 * k <= count) {
            int j = 2 * k;
            if (j + 1 <= count && Data[j + 1] > Data[j]) j++;  //得到当前最大元素的索引
            if (Data[k] >= Data[j]) break;
            swap(Data[k], Data[j]);
            k = j;
        }
    }
    int extractMax() {
        assert(count > 0);
        int ret = Data[1];
        swap(Data[1], Data[count]);
        count--;
        shiftDown(1);
        return ret;
    }

    // 获取最大堆中的堆顶元素
    int getMax() {
        assert(count > 0);
        return Data[1];
    }
private:
	int* Data;
	int count; //*用于计数堆中元素的数量
	int capacity;
	void shiftUp(int k)
	{
		while (k>1 && Data[k/2]<Data[k])
		{
			int tmp = Data[k / 2];    //对树的结构进行调整
			Data[k / 2] = Data[k];
			Data[k] = tmp;
            k = k / 2;
		}
	}

public:
    // 以树状打印整个堆结构
    void testPrint() {

        // 我们的testPrint只能打印100个元素以内的堆的树状信息
        if (size() >= 100) {
            cout << "This print function can only work for less than 100 int";
            return;
        }

        // 我们的testPrint只能处理整数信息


        cout << "The max heap size is: " << size() << endl;
        cout << "Data in the max heap: ";
        for (int i = 1; i <= size(); i++) {
            // 我们的testPrint要求堆中的所有整数在[0, 100)的范围内
            assert(Data[i] >= 0 && Data[i] < 100);
            cout << Data[i] << " ";
        }
        cout << endl;
        cout << endl;

        int n = size();
        int max_level = 0;
        int number_per_level = 1;
        while (n > 0) {
            max_level += 1;
            n -= number_per_level;
            number_per_level *= 2;
        }

        int max_level_number = int(pow(2, max_level - 1));
        int cur_tree_max_level_number = max_level_number;
        int index = 1;
        for (int level = 0; level < max_level; level++) {
            string line1 = string(max_level_number * 3 - 1, ' ');

            int cur_level_number = min(count - int(pow(2, level)) + 1, int(pow(2, level)));
            bool isLeft = true;
            for (int index_cur_level = 0; index_cur_level < cur_level_number; index++, index_cur_level++) {
                putNumberInLine(Data[index], line1, index_cur_level, cur_tree_max_level_number * 3 - 1, isLeft);
                isLeft = !isLeft;
            }
            cout << line1 << endl;

            if (level == max_level - 1)
                break;

            string line2 = string(max_level_number * 3 - 1, ' ');
            for (int index_cur_level = 0; index_cur_level < cur_level_number; index_cur_level++)
                putBranchInLine(line2, index_cur_level, cur_tree_max_level_number * 3 - 1);
            cout << line2 << endl;

            cur_tree_max_level_number /= 2;
        }
    }

private:
    void putNumberInLine(int num, string& line, int index_cur_level, int cur_tree_width, bool isLeft) {

        int sub_tree_width = (cur_tree_width - 1) / 2;
        int offset = index_cur_level * (cur_tree_width + 1) + sub_tree_width;
        assert(offset + 1 < line.size());
        if (num >= 10) {
            line[offset + 0] = '0' + num / 10;
            line[offset + 1] = '0' + num % 10;
        }
        else {
            if (isLeft)
                line[offset + 0] = '0' + num;
            else
                line[offset + 1] = '0' + num;
        }
    }

    void putBranchInLine(string& line, int index_cur_level, int cur_tree_width) {

        int sub_tree_width = (cur_tree_width - 1) / 2;
        int sub_sub_tree_width = (sub_tree_width - 1) / 2;
        int offset_left = index_cur_level * (cur_tree_width + 1) + sub_sub_tree_width;
        assert(offset_left + 1 < line.size());
        int offset_right = index_cur_level * (cur_tree_width + 1) + sub_tree_width + 1 + sub_sub_tree_width;
        assert(offset_right < line.size());

        line[offset_left + 1] = '/';
        line[offset_right + 0] = '\\';
    }

};

MyClass::MyClass(int capacity)
{
	Data = new int[capacity + 1];
	count = 0;
    this->capacity = capacity;
}

MyClass::~MyClass()
{
	delete[] Data;
}
void heapSort(int *arr,int n)
{
    MyClass maxheap = MyClass(n);
    for (int i = 0; i < n; i++)
        maxheap.insert(arr[i]);
    for (int i = n - 1; i >= 0; i--)
        arr[i] = maxheap.extractMax();
}



class IndexMaxHeap {

private:
    int* data;     // 最大索引堆中的数据
    int* indexes;   // 最大索引堆中的索引

    int count;
    int capacity;

    // 索引堆中, 数据之间的比较根据data的大小进行比较, 但实际操作的是索引
    void shiftUp(int k) {

        while (k > 1 && data[indexes[k / 2]] < data[indexes[k]]) {
            swap(indexes[k / 2], indexes[k]);
            k /= 2;
        }
    }

    // 索引堆中, 数据之间的比较根据data的大小进行比较, 但实际操作的是索引
    void shiftDown(int k) {

        while (2 * k <= count) {
            int j = 2 * k;
            if (j + 1 <= count && data[indexes[j + 1]] > data[indexes[j]])
                j += 1;

            if (data[indexes[k]] >= data[indexes[j]])
                break;

            swap(indexes[k], indexes[j]);
            k = j;
        }
    }

public:
    // 构造函数, 构造一个空的索引堆, 可容纳capacity个元素
    IndexMaxHeap(int capacity) {

        data = new int[capacity + 1];
        indexes = new int[capacity + 1];

        count = 0;
        this->capacity = capacity;
    }

    ~IndexMaxHeap() {
        delete[] data;
        delete[] indexes;
    }

    // 返回索引堆中的元素个数
    int size() {
        return count;
    }

    // 返回一个布尔值, 表示索引堆中是否为空
    bool isEmpty() {
        return count == 0;
    }

    // 向最大索引堆中插入一个新的元素, 新元素的索引为i, 元素为item
    // 传入的i对用户而言,是从0索引的
    void insert(int i, int item) {
        assert(count + 1 <= capacity);
        assert(i + 1 >= 1 && i + 1 <= capacity);

        i += 1;
        data[i] = item;
        indexes[count + 1] = i;
        count++;

        shiftUp(count);
    }

    // 从最大索引堆中取出堆顶元素, 即索引堆中所存储的最大数据
    int extractMax() {
        assert(count > 0);

        int ret = data[indexes[1]];
        swap(indexes[1], indexes[count]);
        count--;
        shiftDown(1);
        return ret;
    }

    // 从最大索引堆中取出堆顶元素的索引
    int extractMaxIndex() {
        assert(count > 0);

        int ret = indexes[1] - 1;
        swap(indexes[1], indexes[count]);
        count--;
        shiftDown(1);
        return ret;
    }

    // 获取最大索引堆中的堆顶元素
    int getMax() {
        assert(count > 0);
        return data[indexes[1]];
    }

    // 获取最大索引堆中的堆顶元素的索引
    int getMaxIndex() {
        assert(count > 0);
        return indexes[1] - 1;
    }

    // 获取最大索引堆中索引为i的元素
    int getItem(int i) {
        assert(i + 1 >= 1 && i + 1 <= capacity);
        return data[i + 1];
    }

    // 将最大索引堆中索引为i的元素修改为newItem
    void change(int i, int newItem) {

        i += 1;
        data[i] = newItem;

        // 找到indexes[j] = i, j表示data[i]在堆中的位置
        // 之后shiftUp(j), 再shiftDown(j)
        for (int j = 1; j <= count; j++)
            if (indexes[j] == i) {
                shiftUp(j);
                shiftDown(j);
                return;
            }
    }

    // 测试索引堆中的索引数组index
    // 注意:这个测试在向堆中插入元素以后, 不进行extract操作有效
    bool testIndexes() {

        int* copyIndexes = new int[count + 1];

        for (int i = 0; i <= count; i++)
            copyIndexes[i] = indexes[i];

        copyIndexes[0] = 0;
        std::sort(copyIndexes, copyIndexes + count + 1);

        // 在对索引堆中的索引进行排序后, 应该正好是1...count这count个索引
        bool res = true;
        for (int i = 1; i <= count; i++)
            if (copyIndexes[i - 1] + 1 != copyIndexes[i]) {
                res = false;
                break;
            }

        delete[] copyIndexes;

        if (!res) {
            cout << "Error!" << endl;
            return false;
        }

        return true;
    }
};
void heapSortUsingIndexMaxHeap(int arr[], int n) {

    IndexMaxHeap indexMaxHeap = IndexMaxHeap(n);
    for (int i = 0; i < n; i++)
        indexMaxHeap.insert(i, arr[i]);
    assert(indexMaxHeap.testIndexes());

    for (int i = n - 1; i >= 0; i--)
        arr[i] = indexMaxHeap.extractMax();
}
int main()
{
    int n = 10000;
    int* arr = SortTestHelper::generateRandomArray(n, 0, n);
    SortTestHelper::testSort("Heap Sort Using Index-Max-Heap", heapSortUsingIndexMaxHeap, arr, n);
    delete[] arr;

}
