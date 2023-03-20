#ifndef PROGRMMING_FUNCTION_H
#define PROGRMMING_FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Quadtree.h"

#define MAX_FILE_LENGTH 100 // 文件路径的最大长度
#define MAX_FILE_COUNT 5    // 一次读入的最大文件数量
#define MAX_BS_COUNT 10000  // 一次读入的最大基站数量
#define MAX_MT_COUNT 50     // 终端最大的数据数
#define INACCURACY 0.01     // 定义迭代的误差
#define TIME_STEP 1.0       // 定义基站和伪基站了连接的时候的时间步长

char jz_filename[MAX_FILE_COUNT][MAX_FILE_LENGTH]; // 多个基站文件名称
char zd_filename[MAX_FILE_LENGTH];                 // 移动终端文件名称
char wz_filename[MAX_FILE_LENGTH];                 // 伪基站位置文件名称

typedef struct MobileTerminal
{
    int x_start, y_start;         // 终端的起始位置
    int x_end, y_end;             // 终端的终止位置
    int speed;                    // 终端的速度
    int start_hour, start_minute; // 终端移动开始的时间
} MobileTerminal;                 // 存储读入的终端数据

typedef struct Fake_Base_Station
{
    int x_start, y_start;         // 伪基站的起始位置
    int x_end, y_end;             // 伪基站的终止位置
    int speed;                    // 伪基站的速度
    int start_hour, start_minute; // 伪基站移动开始的时间
    int num;                      // 伪基站的编号
} Fake_Base_Station;              // 存储读入的伪基站的数据

static int base_count = 0;                          // 记录基站的数量
static int mobile_count = 0;                        // 记录移动终端的数据量
static int fake_base_count = 0;                     // 记录伪基站的数量
static double step = 20;                            // 记录检查间隔的步长
BASE Bases[MAX_BS_COUNT];                           // 存储读入的基站数据
MobileTerminal MobileTerminals[MAX_MT_COUNT];       // 存储读入的终端数据
Fake_Base_Station Fake_Base_Stations[MAX_MT_COUNT]; // 存储读入的伪基站的数据

QuadTreeNode *root;             // 四叉树的根结点
double up, bottom, left, right; // 四叉树的边界值

static double max_Urban_radius = 0;    // 城市区域的最大有效半径
static double max_Township_radius = 0; // 城镇区域的最大有效半径
static double max_High_way_radius = 0; // 高速区域的最大有效半径

static int urban_count = 0;
static int township_count = 0;
static int high_way_count = 0;

/*----------------------------------------------------------------*/

/****** 文件数据的读入的功能 ******/

/*** 基站(BASE)和移动终端(Mobile Terminal)的输入总控函数 ***/
void BS_MT_Input();

/*** 用于删除传入字符串中的空白字符，以便进行字符串比较 ***/
void trim(char *str);

/*** 用于实现基站文件数据的读入 ***/
void BS_file_input();

/*** 用于实现终端数据的读入 ***/
void MT_file_input();

/*** 用于实现伪基站数据的读入 ***/
void FAKE_BS_file_input();

/****** 文件数据的读入功能 ******/

/*----------------------------------------------------------------*/

/****** 文件数据的清除功能 ******/

/*** 清除基站和移动终端数据 ***/
void erase_BS_MT();

/*** 清除伪基站的数据 ***/
void erase_Fake_BS();

/****** 文件数据的清除功能 ******/

/*----------------------------------------------------------------*/

/****** 四叉树的构建功能 ******/

/*** 根据基站数据完成四叉树的生成 ***/
void Build_QuadTree();

/****** 四叉树的构建功能 ******/

/*----------------------------------------------------------------*/

/****** 主要功能1 ******/

/***  主要功能1的对外接口***/
void main_func1();

/*** 传入四叉树的根结点，和所选方位，展示该方位区域内的所有基站 ***/
void show_corner(QuadTreeNode *node, char pos[]);

/****** 主要功能1 ******/

/*----------------------------------------------------------------*/

/****** 主要功能2 ******/

/*** 主要功能2的对外接口 ***/
void main_func2();

/*** 显示出西北角分块/树叶的东侧分块/树叶基站数据 ***/
void show_LU_east();

/*** 显示出西北角分块/树叶的南侧分块/树叶基站数据 ***/
void show_LU_south();

/*** 显示出东南角所在分块/树叶的西北侧基站数据 ***/
void show_RB_LU();

/*** 显示出东南角所在分块/树叶的西北侧的北侧相邻分块/树叶的基站数据 ***/
void show_RB_LU_north();

/****** 主要功能2 ******/

/*----------------------------------------------------------------*/

/****** 主要功能3 ******/

/*** 主要功能3的对外接口 ***/
void main_func3();

/*** 查询并输出(x,y)处信号最强的基站信息 ***/
BASE *find_best_base(double x, double y);

/*** 遍历基站得到不同类型基站的最大有效半径 ***/
void get_max_radius();

/*** 判断(x,y)附近的两个基站信号强度的大小关系 ***/
bool is_better(BASE *base1, BASE *base2, double x, double y);

/*** 计算一个基站对于x, y处点的信号强度 ***/
double get_signal_strength(BASE *base, double x, double y);

/*** 传入两个点的坐标，计算两点之间的距离 ***/
double get_distance(double x1, double y1, double x2, double y2);

/****** 主要功能3 ******/

/*----------------------------------------------------------------*/

/****** 主要功能4 ******/

/*** 实现根据终端的移动轨迹分析应该连接的基站时间序列 ***/
void main_func4();

/*** 输出第i条终端移动轨迹的基站连接情况 ***/
void output_result(int i);

/****** 主要功能4 ******/

/*----------------------------------------------------------------*/

/****** 查找给定区域内基站 ******/

/*** 实现查找任意区域内的所有基站信息 ***/
void main_func5();

/****** 查找给定区域内基站 ******/

/*----------------------------------------------------------------*/

/****** 扩展功能1 ******/

void extended_func1();

/****** 扩展功能1 ******/

/*----------------------------------------------------------------*/

/****** 扩展功能2 ******/

void extended_func2();

/****** 扩展功能2 ******/

/*----------------------------------------------------------------*/

/****** 升级功能 ******/

void upgrade_func();

/****** 升级功能 ******/

/*----------------------------------------------------------------*/

#endif // PROGRMMING_FUNCTION_H
