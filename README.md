###  github

#author YupingXia
#version v1.0
##### 一、字符串和数组
给你一个整数数组 nums,请计算数组的中心下标。数组中心下标是数组的一个下标，其左侧所有元素相加的和等于右侧所有元素相加的和。
1. 寻找数组的中心索引
* 第一种思想：通过循环遍历，分别计算数组下标左右元素之和，时间复杂度为o(n^2^),代码如下：

  ```C++
      int pivotIndex(vector<int>& nums)
      {
          int lSum = 0, rSum = 0;
          for (int i = 0;i < nums.size();i++)
          {  
              for (int j = 0;j < i;j++)//计算下标左侧和
              {
                  lSum += nums[j];
              }
              for (int j = i + 1;j < nums.size();j++)//计算下标右侧和
              {
                  rSum += nums[j];
              }
              if (lSum == rSum) return i;
              lSum = 0, rSum = 0;            //没找到时，初始值赋为0，继续寻找
          }
          return -1;
      }
  ```
