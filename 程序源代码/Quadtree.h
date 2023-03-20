#ifndef PROGRMMING_QUADTREE_H
#define PROGRMMING_QUADTREE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_leaf_BS_COUNT 6 // 一个最小的区域内存储的最多的基站个数

// 上下左右边界区域的结构体
struct Region
{
    double up;
    double bottom;
    double left;
    double right;
};

// 基站结构数组用于存储基站文件的导入
typedef struct BS
{
    int x, y;      // 基站的x,y坐标
    char type[10]; // 基站的类型
    double power;  // 基站的功率
    double radius; // 由功率算出有效半径
    int num;       // 基站的编号
} BASE;

// 定义四叉树节点的结构体
typedef struct QuadTreeNode
{
    struct QuadTreeNode *LU;            // 左上子结点指针
    struct QuadTreeNode *LB;            // 左下子结点指针
    struct QuadTreeNode *RU;            // 右上子结点指针
    struct QuadTreeNode *RB;            // 右下子结点指针
    struct QuadTreeNode *father;        // 结点的父亲结点
    struct Region region;               // 区域范围
    int level;                          // 结点所属层级
    bool is_leaf;                       // 判断是否是叶子结点
    int base_count;                     // 存储现有基站个数
    struct BS bases[MAX_leaf_BS_COUNT]; // 区域中的基站个数
} QuadTreeNode;

/******  得到整体数据的边界值，用于构建四叉树的根节点******/
void get_boundary(BASE *base, int base_count, double *up, double *bottom, double *left, double *right);

/****** 初始化四叉树节点 ******/
QuadTreeNode *init_node(QuadTreeNode *father, double up, double bottom, double left, double right, int level);

/****** 递归地构建四叉树，并且返回树根 ******/
QuadTreeNode *build_tree(BASE *base, int base_count, QuadTreeNode *father, double up, double bottom, double left, double right, int level);

/****** 释放四叉树的所有空间 ******/
void free_tree(QuadTreeNode *node);

/****** 根据所给的区域进行递归查找特定类型的基站 ******/
void search_tree(QuadTreeNode *node, double up, double bottom, double left, double right, BASE *result, int *result_count, char *type);

#endif // PROGRMMING_QUADTREE_H
