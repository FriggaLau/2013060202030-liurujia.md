#include <iostream>
#include <cmath>
#include <string>
#include <sys/types.h>

using namespace std;


typedef unsigned int size_32;//虚拟地址为32位
const int INDIRECT_BLOCK_ITEM_SIZE = 10;//间接块里有10个元素
const int BLOCK_SIZE = 1024;//BLOCK大小为1024（即1KB）
//逻辑块n对应物理块n
//文件按顺序占用物理块


// FirstLevelIndirectBlock 
struct FirstLevelIndirectBlock
{
    int iaddr[INDIRECT_BLOCK_ITEM_SIZE];

    FirstLevelIndirectBlock()
    {
        for (int i = 0; i < INDIRECT_BLOCK_ITEM_SIZE; ++i)
            iaddr[i] = -1;
    }
};

// SecondLevelIndirectBlock
struct SecondLevelIndirectBlock
{
    struct FirstLevelIndirectBlock FirstLevelIndirectBlocks[INDIRECT_BLOCK_ITEM_SIZE];
};

// ThirdLevelIndirectBlock
struct ThirdLevelIndirectBlock
{
    struct SecondLevelIndirectBlock SecondLevelIndirectBlocks[INDIRECT_BLOCK_ITEM_SIZE];
};

struct inode//这里的inode并不等同于真实的inode 只是一个概念上的结构
{
    int mode; // 访问权限
    string owners; // 持有者
    int timeStamps; // 创建时间
    int iaddr[INDIRECT_BLOCK_ITEM_SIZE];
    struct FirstLevelIndirectBlock sib;
    struct SecondLevelIndirectBlock dib;
    struct ThirdLevelIndirectBlock tib;

    inode() : mode(-1), owners(""), timeStamps(-1)
    {
        for (int i = 0; i < INDIRECT_BLOCK_ITEM_SIZE; ++i)
            iaddr[i] = -1;
    }
};

void initInode(struct inode& node)//初始化
{
    node.mode = 644;
    node.owners = "刘汝佳";
    node.timeStamps = 20160608;

    int count = 0;
    for (int i = 0; i < INDIRECT_BLOCK_ITEM_SIZE; ++i)
        node.iaddr[i] = count++;//初始化直接索引
    for (int i = 0; i < INDIRECT_BLOCK_ITEM_SIZE; ++i)
        node.sib.iaddr[i] = count++;//初始化一次间接块
    for (int i = 0; i < INDIRECT_BLOCK_ITEM_SIZE; ++i)
        for (int j = 0; j < INDIRECT_BLOCK_ITEM_SIZE; ++j)
            node.dib.FirstLevelIndirectBlocks[i].iaddr[j] = count++;//初始化二次间接块

//三次间接块暂时不使用
}

void printInode(const struct inode& node)//输出Inode
{
    cout << "权限: " << node.mode << endl;
    cout << "持有者: " << node.owners << endl;
    cout << "创建时间: " << node.timeStamps << endl;
    cout << "直接块: ";
    for (int i = 0; node.iaddr[i] != -1 && i < INDIRECT_BLOCK_ITEM_SIZE; ++i)
        cout << node.iaddr[i] << " ";
    cout << endl;
}

int convertToPhysicalAddr(size_32 logicAddr, const struct inode& node,
        size_32& physicalBlockNo, size_32& physicalBlockOffset)
{
    size_32 idx = logicAddr / BLOCK_SIZE;
    physicalBlockOffset = logicAddr % BLOCK_SIZE;

    // 直接索引
    if (idx < 10)
    {
        if (node.iaddr[idx] == -1)
            return -1;
        physicalBlockNo = node.iaddr[idx];
        return 0;
    }
    // 一次间接块
    else if (idx < 10 + INDIRECT_BLOCK_ITEM_SIZE)
    {
        idx = idx - 10;
        if (node.sib.iaddr[idx] == -1)
            return -1;
        physicalBlockNo = node.sib.iaddr[idx];
        return 0;
    }
    // 二次间接块
    else if (idx < 10 + INDIRECT_BLOCK_ITEM_SIZE + INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE)
    {
        int i, j;
        idx = idx - 10 - INDIRECT_BLOCK_ITEM_SIZE;
        i = idx / INDIRECT_BLOCK_ITEM_SIZE;
        j = idx % INDIRECT_BLOCK_ITEM_SIZE;
        if (node.dib.FirstLevelIndirectBlocks[i].iaddr[j] == -1)
            return -1;
        physicalBlockNo = node.dib.FirstLevelIndirectBlocks[i].iaddr[j];
        return 0;
    }
    // 三次间接块
    else if (10 + INDIRECT_BLOCK_ITEM_SIZE + INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE + INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE)
    {
        int i, j, k;
        idx = idx - 10 - INDIRECT_BLOCK_ITEM_SIZE - INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE;

        i = idx / (INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE);
        idx %= (INDIRECT_BLOCK_ITEM_SIZE * INDIRECT_BLOCK_ITEM_SIZE);
        j = idx / INDIRECT_BLOCK_ITEM_SIZE;
        k = idx % INDIRECT_BLOCK_ITEM_SIZE;
        if (node.tib.SecondLevelIndirectBlocks[i].FirstLevelIndirectBlocks[j].iaddr[k] == -1)
            return -1;
        physicalBlockNo = node.tib.SecondLevelIndirectBlocks[i].FirstLevelIndirectBlocks[j].iaddr[k];
        return 0;
    }
    else
        return -1;

    return 0;
}

int main(int argc, char *argv[])//主函数
{
    struct inode node;
    initInode(node);
    printInode(node);

    size_32 logicAddr;
    size_32 physicalBlockNo;
    size_32 physicalBlockOffset;

    cout << "请输入文件逻辑地址" << endl;
    cin >> hex >> logicAddr;

    int ret = convertToPhysicalAddr(logicAddr, node, physicalBlockNo, physicalBlockOffset);

    cout << "inode详细信息: " << ret << endl;
    if (ret == 0)
    {
        cout << "物理块号: " << physicalBlockNo << endl;
        cout << "块内偏移: " << physicalBlockOffset << endl;
    }

    return 0;
}