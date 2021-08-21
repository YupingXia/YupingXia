//
// Created by liuyubobobo on 7/12/16.
//


#ifndef INC_03_SELECTION_SORT_DETECT_PERFORMANCE_SORTTESTHELPER_H
#define INC_03_SELECTION_SORT_DETECT_PERFORMANCE_SORTTESTHELPER_H

#include <iostream>
#include <ctime>
#include <cassert>
#include <string>

using namespace std;


namespace SortTestHelper {

    // ������n��Ԫ�ص��������,ÿ��Ԫ�ص������ΧΪ[rangeL, rangeR]
    int* generateRandomArray(int n, int rangeL, int rangeR) {

        assert(rangeL <= rangeR);

        int* arr = new int[n];

        srand(time(NULL));
        for (int i = 0; i < n; i++)
            arr[i] = rand() % (rangeR - rangeL + 1) + rangeL;
        return arr;
    }

    // ��ӡarr�������������
    template<typename T>
    void printArray(T arr[], int n) {

        for (int i = 0; i < n; i++)
            cout << arr[i] << " ";
        cout << endl;

        return;
    }

    // �ж�arr�����Ƿ�����
    template<typename T>
    bool isSorted(T arr[], int n) {

        for (int i = 0; i < n - 1; i++)
            if (arr[i] > arr[i + 1])
                return false;

        return true;
    }

    // ����sort�����㷨����arr�������õ��������ȷ�Ժ��㷨����ʱ��
    // * ʹ��VS�����ͬѧ, ���ں���ָ���д���͵��÷������ܺͿγ��н��ܵ�������ͬ;
    // * ���Ҳ�ͬ�汾��VS, ������﷨����Ҳ�в���, ������ΪVS�ı���������ȫ�ǰ���C++�ı�׼ʵ�ֵ�;
    // * ���γ̰���C++11�ı�׼������д������VS�������������﷨����, ϣ��ͬѧ�ǿ����Լ������ϲ���������Ͻ��;
    // * ���Ҳ�����ڿγ̵Ĺٷ�QQȺ�н���
    // * ����, ʹ�ú���ָ�뱾����������γ̵��ص�, ���Ҳ��ȫ����ʹ���Լ��ķ�ʽ��д����, ����ֻҪ�ܹ����Գ��Լ���д���㷨��Ч�ʼ���
    // * �Ƽ�����Ķ������ʴ������ҷ����һ��ѧϰ�ĵ�: ��ѧϰ�ĵ÷�������ץ���С����Ҫ������C++���Ե��﷨ϸ��
    // * ����: http://coding.imooc.com/learn/questiondetail/4100.html
    template<typename T>
    void testSort(const string& sortName, void (*sort)(T[], int), T arr[], int n) {

        clock_t startTime = clock();
        sort(arr, n);
        clock_t endTime = clock();

        assert(isSorted(arr, n));
        cout << sortName << " : " << double(endTime - startTime) / CLOCKS_PER_SEC << " s" << endl;

        return;
    }

};
#endif //INC_03_SELECTION_SORT_DETECT_PERFORMANCE_SORTTESTHELPER_H#pragma once
