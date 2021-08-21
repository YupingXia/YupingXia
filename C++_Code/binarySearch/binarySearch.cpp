// binarySearch.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cassert>
#include <ctime>
#include<stdio.h>
#include<queue>
using namespace std;
int binarySearch(int *a,int n,int target)
{
    int left = 0; int right = n - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (a[mid] == target)
            return mid;
        if (a[mid]<target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}
int binarySearch1(int* a,int left,int right, int target)
{
    if (left > right)
        return -1;
    int mid = left + (right - left) / 2;
    if (a[mid] == target)
        return mid;

    if (a[mid] < target)
    {
        binarySearch1(a, mid + 1, right, target);
    }
    else
    {
        binarySearch1(a, left, mid - 1, target);
    }
}

template <typename Key, typename Value>
class BST {

private:
    // 二分搜索树中的节点为私有的结构体, 外界不需要了解二分搜索树节点的具体实现
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;

        Node(Key key, Value value) {   //空节点时候使用
            this->key = key;
            this->value = value;
            this->left = this->right = NULL;
        }
    };
    Node* root; // 根节点
    int count;  // 节点个数
    Node* insert(Node *node,Key key,Value value)  //只要是小的，一直往左子树放；
    {
        if (node==NULL)
        {
            count++;
            return new Node(key, value);
        }
        if (key==node->key)
        {
            node->value = value;
        }
        else if (key<node->key)
        {
            node->left = insert(node->left, key, value);
        }
        else
        {
            node->right = insert(node->right, key, value);
        }
        return node;
    }
    bool contain(Node* node, Key key)
    {
        if (node == NULL)
        {
            return false;
        }
        if (key = node->key)
        {
            return true;
        }
        else if (key<node->key)
        {
            return contain(node->left,key);
        }
        else
        {
            return contain(node->right, key);
        }
    }
    Value* searh(Node* node, Key key)
    {
        if (node==NULL)
        {
            return NULL;
        }
        if (key=node->key)
        {
            return &(node->value);
        }
        else if (key<node->key)
        {
            return search(node->left,key);
        }
        else
        {
            return search(node->right, key);
        }
    }
    void preOrder(Node* node)
    {
        if (node!=NULL)
        {
            cout << node->key << ",";
            preOrder(node->left);
            preOrder(node->right);
        }
    }
    void inOrder(Node* node)
    {
        if (node!=NULL)
        {
            inOrder(node->left);
            cout << node->key << ",";
            inOrder(node->right);
        }
    }
    void postOrder(Node* node)
    {
        if (node!=NULL)
        {
            postOrder(node->left);
            postOrder(node->right);
            cout << node->key << ",";
        }
    }
    void destory(Node* node)
    {
        if (node!=NULL)
        {
            destory(node->left);
            destory(node->right);

            delete node;
            count--;
        }
    }
    Node* minimum(Node *node)
    {
        if (node->left == NULL)
            return node;
        return minimum(node->left);
    }
    Node* maximum(Node* node)
    {
        if (node->right==NULL)
        {
            return node;
        }
        return maximum(node->right);
    }
    Node* removeMin(Node* node)
    {
        if (node->left==NULL)
        {
            Node* rightNode = node->right;
            delete node;
            count--;
            return rightNode;
        }
        node->left = removeMin(node->left);
        return node;
    }
    Node* removeMax(Node* node)
    {
        if (node->right == NULL)
        {
            Node* leftNode = node->left;
            delete node;
            count--;
            return leftNode;
        }
        node->right = removeMax(node->right);
        return node;
    }

public:
// 构造函数, 默认构造一棵空二分搜索树
    BST() {
        root = NULL;
        count = 0;
    }
    ~BST() {
        // TODO: ~BST()
    }

    // 返回二分搜索树的节点个数
    int size() {
        return count;
    }

    // 返回二分搜索树是否为空
    bool isEmpty() {
        return count == 0;
    }

    void insert(Key key, Value value)
    {
        root = insert(root,key,value);
    }
    bool contain(Key key) {
        return contain(root, key);
    }

    // 在二分搜索树中搜索键key所对应的值。如果这个值不存在, 则返回NULL
    Value* search(Key key) {
        return search(root, key);
    }

    // 二分搜索树的前序遍历
    void preOrder() {
        preOrder(root);
    }

    // 二分搜索树的中序遍历
    void inOrder() {
        inOrder(root);
    }

    // 二分搜索树的后序遍历
    void postOrder() {
        postOrder(root);
    }
    void levelOrder()
    {
        if (root==NULL)
        {
            return ;
        }
        queue<Node*>q;
        q.push(root);
        while (!q.empty())
        {
            Node* node = q.front();
            q.pop();

            cout << node->key << endl;
            if (node->left)
            {
                q.push(node->left);
            }
            if (node->right)
            {
                q.push(node->right);

            }
        }
    }
    Key minimum()
    {
        assert(count != 0);
        Node* minNode = minimum(root);
        return minNode->key;
    }
    Key maximum()
    {
        assert(count != 0);
        Node* maxNode = maximum(root);
        return maxNode->key;
    }
    void removeMin()
    {
        if (root)
            root = removeMin(root);
    }
    void removeMax()
    {
        if (root)
        {
            root = removeMax(root);
        }
    }
};

int main()
{
    BST<int, int> bst = BST<int, int>();
    int N = 10;
    int M = 100;
    for (int i = 0; i < N; i++)
    {
        int key = rand() % M;
        int value = key;
        cout << key << " ";
        bst.insert(key, value);
    }
    cout << endl;
    /*
    cout << endl;
    cout << "size: " << bst.size() << endl << endl;

    cout << "preOrder: " << endl;
    bst.preOrder();
    cout << endl;

    cout << "inOrder: " << endl;
    bst.inOrder();
    cout << endl;

    cout << "postOrder: " << endl;
    bst.postOrder();
    cout << endl;
     */
    cout << "levelOrder: " << endl;
    bst.levelOrder();
    cout << endl;
   
    cout << "removeMin Test:" << endl;
    while (!bst.isEmpty())
    {
        cout << "min:" << bst.minimum() << ",";
        bst.removeMin();
        cout << "After removeMin,size= " << bst.size() << endl;
    }
    cout << endl;


    for (int i = 0; i < N; i++)
    {
        int key = rand() % M;
        int value = key;
        cout << key << " ";
        bst.insert(key, value);
    }
    cout << endl;
    cout << "removeMax Test:" << endl;
    while (!bst.isEmpty())
    {
        cout << "max:" << bst.maximum() << ",";
        bst.removeMax();
        cout << "After removeMax,size= " << bst.size() << endl;
    }
    cout << endl;



    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
