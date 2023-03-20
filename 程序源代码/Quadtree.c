#include "Quadtree.h"

/****** 得到整体数据的边界值，用于构建四叉树的根节点 ******/
void get_boundary(BASE *base, int base_count, double *up, double *bottom, double *left, double *right)
{
    *up = base[0].y;
    *bottom = base[0].y;
    *left = base[0].x;
    *right = base[0].x;
    for (int i = 1; i < base_count; i++)
    {
        if (base[i].y > *up)
            *up = base[i].y;
        if (base[i].y < *bottom)
            *bottom = base[i].y;
        if (base[i].x < *left)
            *left = base[i].x;
        if (base[i].x > *right)
            *right = base[i].x;
    }
}

/****** 初始化四叉树节点 ******/
QuadTreeNode *init_node(QuadTreeNode *father, double up, double bottom, double left, double right, int level)
{
    QuadTreeNode *node = (QuadTreeNode *)malloc(sizeof(QuadTreeNode));
    node->LU = NULL;
    node->LB = NULL;
    node->RU = NULL;
    node->RB = NULL;
    node->region.up = up;
    node->region.bottom = bottom;
    node->region.left = left;
    node->region.right = right;
    node->level = level;
    node->is_leaf = true;
    node->base_count = 0;
    node->father = father;
    return node;
}

/****** 递归地构建四叉树，并且返回树根 ******/
QuadTreeNode *build_tree(BASE *base, int base_count, QuadTreeNode *father, double up, double bottom, double left, double right, int level)
{
    QuadTreeNode *node = init_node(father, up, bottom, left, right, level); // 初始化树根结点
    if (base_count <= MAX_leaf_BS_COUNT)                                    // 如果该区域目前能够容纳base_count个基站的话
    {
        for (int i = 0; i < base_count; i++) // 遍历这些基站并进行赋值
            node->bases[i] = base[i];
        node->base_count = base_count;
        return node; // 返回树根
    }
    else // 另一种情况就是容纳不下了，需要另外开辟空间去分裂这些结点
    {
        // 计算区域中心
        double mid_x = (left + right) / 2;
        double mid_y = (up + bottom) / 2;
        // 先创建四个区域
        BASE *base_LU = (BASE *)malloc(sizeof(BASE) * base_count);
        BASE *base_LB = (BASE *)malloc(sizeof(BASE) * base_count);
        BASE *base_RU = (BASE *)malloc(sizeof(BASE) * base_count);
        BASE *base_RB = (BASE *)malloc(sizeof(BASE) * base_count);
        int base_LU_count = 0, base_LB_count = 0, base_RU_count = 0, base_RB_count = 0; // 用于统计分裂后的四个区域的基站个数
        for (int i = 0; i < base_count; i++)                                            // 遍历判断基站的归属
        {
            if (base[i].x <= mid_x && base[i].y >= mid_y)
            {
                base_LU[base_LU_count++] = base[i];
            }
            else if (base[i].x <= mid_x && base[i].y <= mid_y)
            {
                base_LB[base_LB_count++] = base[i];
            }
            else if (base[i].x >= mid_x && base[i].y >= mid_y)
            {
                base_RU[base_RU_count++] = base[i];
            }
            else if (base[i].x >= mid_x && base[i].y <= mid_y)
            {
                base_RB[base_RB_count++] = base[i];
            }
        }
        // 递归构建四个区域的四叉树
        node->LU = build_tree(base_LU, base_LU_count, node, up, mid_y, left, mid_x, level + 1);
        node->LB = build_tree(base_LB, base_LB_count, node, mid_y, bottom, left, mid_x, level + 1);
        node->RU = build_tree(base_RU, base_RU_count, node, up, mid_y, mid_x, right, level + 1);
        node->RB = build_tree(base_RB, base_RB_count, node, mid_y, bottom, mid_x, right, level + 1);
        node->is_leaf = false; // 因为分裂产生了四个区域，所以这个结点并非叶子结点
        // 释放中间使用到的指针
        free(base_LU);
        free(base_LB);
        free(base_RU);
        free(base_RB);
        return node;
    }
}

/****** 释放四叉树的所有空间 ******/
void free_tree(QuadTreeNode *node)
{
    if (node->is_leaf)
    {
        free(node);
    }
    else
    {
        free_tree(node->LU);
        free_tree(node->LB);
        free_tree(node->RU);
        free_tree(node->RB);
        free(node);
    }
}

/****** 根据所给的区域进行递归查找 ******/
void search_tree(QuadTreeNode *node, double up, double bottom, double left, double right, BASE *result, int *result_count, char *type)
{
    // 递归结束的条件，该结点为叶子的时候，符合条件的基站加入result中
    if (node->is_leaf)
    {
        for (int i = 0; i < node->base_count; i++)
        {
            if (node->bases[i].x >= left && node->bases[i].x <= right && node->bases[i].y >= bottom && node->bases[i].y <= up && strcmp(node->bases[i].type, type) == 0)
            {
                result[(*result_count)++] = node->bases[i];
            }
        }
    }
    // 如果不是叶子结点，那么就需要递归查找只要用重叠的部分
    else
    {
        if (node->LU != NULL && node->LU->region.up >= bottom && node->LU->region.bottom <= up && node->LU->region.left <= right && node->LU->region.right >= left)
        {
            search_tree(node->LU, up, bottom, left, right, result, result_count, type);
        }
        if (node->LB != NULL && node->LB->region.up >= bottom && node->LB->region.bottom <= up && node->LB->region.left <= right && node->LB->region.right >= left)
        {
            search_tree(node->LB, up, bottom, left, right, result, result_count, type);
        }
        if (node->RU != NULL && node->RU->region.up >= bottom && node->RU->region.bottom <= up && node->RU->region.left <= right && node->RU->region.right >= left)
        {
            search_tree(node->RU, up, bottom, left, right, result, result_count, type);
        }
        if (node->RB != NULL && node->RB->region.up >= bottom && node->RB->region.bottom <= up && node->RB->region.left <= right && node->RB->region.right >= left)
        {
            search_tree(node->RB, up, bottom, left, right, result, result_count, type);
        }
    }
}
