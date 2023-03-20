#include "function.h"

/****** 文件数据的读入功能 ******/

/*** 基站(BASE)和移动终端(Mobile Terminal)的输入总控函数 ***/
void BS_MT_Input()
{
    // 第一步实现基站的数据读入
    BS_file_input();
    // 第二步实现移动终端的数据读入
    MT_file_input();
}

/*** 用于删除传入字符串中的空白字符，以便进行字符串比较 ***/
void trim(char *str)
{
    char *str_c = str;
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < 'A' || str[i] > 'Z')
            continue;
        str_c[j++] = str[i];
    }
    str_c[j] = '\0';
}

/*** 用于实现基站文件数据的读入 ***/
void BS_file_input()
{
    printf("Step1: 基站文件的导入:\n");
    int file_count; // 读入文件的个数
    if (base_count)
        printf("您此前已导入%d个基站数据!\n", base_count);
    printf("请输入需要导入的基站文件个数: ");
    scanf("%d", &file_count);

    for (int i = 0; i < file_count;)
    {
        printf("请输入第%d个文件的路径: ", i + 1);
        scanf("%s", jz_filename[i]);
        int flag = 1;                    // 判断文件是否导入成功的标志
        FILE *fp = NULL;                 // 声明文件指针
        char buff[255];                  // 按行读取文件
        fp = fopen(jz_filename[i], "r"); // 打开存储的文件
        if (fp == NULL)                  // 文件打开失败特判
        {
            printf("打开文件失败，请检查文件路径!\n");
            flag = 0;
        }

        if (flag) // 打开文件失败直接跳过该步骤
        {
            fgets(buff, 255, (FILE *)fp);
            trim(buff);                  // 去除字符串中的空白字符
            if (strcmp("JZ", buff) != 0) // 判断是否为基站文件
            {
                printf("文件所包含数据不是基站数据，请输入正确的文件类型!\n");
                flag = 0;
            }
            if (flag) // 文件类型不对直接跳过该步骤
            {
                int pre = base_count;
                while (true) // 读入基站数据
                {
                    int x, y, num; // x,y为坐标，num为基站编号，power为基站功率, type为基站类型
                    double power;
                    char type[10];
                    fscanf(fp, "%d,%d,%s %lf,%d", &x, &y, type, &power, &num);
                    if (x == -1 && y == -1)
                        break;
                    Bases[base_count].x = x;
                    Bases[base_count].y = y;
                    strcpy(Bases[base_count].type, type);
                    Bases[base_count].power = power;
                    Bases[base_count].num = num;
                    // 根据基站类型和基站功率计算出有效半径
                    if (strcmp(type, "Urban") == 0)
                        Bases[base_count].radius = sqrt(power) * 300;
                    else if (strcmp(type, "Township") == 0)
                        Bases[base_count].radius = sqrt(power) * 1000;
                    else
                        Bases[base_count].radius = sqrt(power) * 5000;
                    base_count++;
                }
                if (pre == 0)
                    printf("已成功导入%d条基站数据!\n", base_count);
                else
                    printf("本次成功导入%d条基站数据,共导入%d条基站数据!\n", base_count - pre, base_count);
                i++;
                fclose(fp);
            }
        }
    }
}

/*** 用于实现终端数据的读入 ***/
void MT_file_input()
{
    printf("Step2: 终端文件的导入:\n");
    FILE *fp = NULL; // 声明文件指针
    char buff[255];  // 按行读取文件
    int flag = 0;    // 判断文件是否导入成功的标志
    while (flag == 0)
    {
        printf("请输入终端文件的路径: ");
        int sign = 1;
        scanf("%s", zd_filename);
        fp = fopen(zd_filename, "r"); // 打开存储的文件
        if (fp == NULL)               // 文件打开失败特判
        {
            printf("打开文件失败，请检查文件路径!\n");
            sign = 0;
        }
        if (sign) // 打开文件失败直接跳过该步骤
        {
            fgets(buff, 255, (FILE *)fp);
            trim(buff);                  // 去除字符串中的空白字符
            if (strcmp("YD", buff) != 0) // 判断是否为基站文件
            {
                printf("文件所包含数据不是终端数据，请输入正确的文件类型!\n");
                sign = 0;
            }
            if (sign) // 文件类型不对直接跳过该步骤
            {
                while (true) // 读入基站数据
                {
                    int x_start, y_start, x_end, y_end; // 移动终端的起点和终点坐标
                    int speed;                          // 移动终端的速度
                    int start_hour, start_minute;       // 移动终端的起始时间
                    fscanf(fp, "%d,%d,%d,%d,%d,%d,%d", &x_start, &y_start, &x_end, &y_end, &speed, &start_hour, &start_minute);
                    if (x_start == -1 && y_start == -1)
                        break;
                    MobileTerminals[mobile_count].x_start = x_start;
                    MobileTerminals[mobile_count].y_start = y_start;
                    MobileTerminals[mobile_count].x_end = x_end;
                    MobileTerminals[mobile_count].y_end = y_end;
                    MobileTerminals[mobile_count].speed = speed;
                    MobileTerminals[mobile_count].start_hour = start_hour;
                    MobileTerminals[mobile_count].start_minute = start_minute;
                    mobile_count++;
                }
                printf("已成功导入移动终端数据!\n");
                flag = 1;
                fclose(fp);
            }
        }
    }
}

/*** 用于实现伪基站数据的读入 ***/
void FAKE_BS_file_input()
{
    printf("Step3: 伪基站文件的导入:\n");
    FILE *fp = NULL; // 声明文件指针
    char buff[255];  // 按行读取文件
    int flag = 0;    // 判断文件是否导入成功的标志
    while (flag == 0)
    {
        printf("请输入伪基站文件的路径: ");
        int sign = 1;
        scanf("%s", wz_filename);
        fp = fopen(wz_filename, "r"); // 打开存储的文件
        if (fp == NULL)               // 文件打开失败特判
        {
            printf("打开文件失败，请检查文件路径!\n");
            sign = 0;
        }
        if (sign) // 打开文件失败直接跳过该步骤
        {
            fgets(buff, 255, (FILE *)fp);
            trim(buff);                  // 去除字符串中的空白字符
            if (strcmp("WZ", buff) != 0) // 判断是否为基站文件
            {
                printf("文件所包含数据不是伪基站数据，请输入正确的文件类型!\n");
                sign = 0;
            }
            if (sign) // 文件类型不对直接跳过该步骤
            {
                while (true) // 读入基站数据
                {
                    int x_start, y_start, x_end, y_end; // 伪基站的起点和终点坐标
                    int speed;                          // 伪基站的速度
                    int start_hour, start_minute;       // 伪基站的起始时间
                    int num;                            // 伪基站的编号
                    fscanf(fp, "%d,%d,%d,%d,%d,%d,%d,%d", &x_start, &y_start, &x_end, &y_end, &speed, &start_hour,
                           &start_minute, &num);
                    if (x_start == -1 && y_start == -1)
                        break;
                    Fake_Base_Stations[fake_base_count].x_start = x_start;
                    Fake_Base_Stations[fake_base_count].y_start = y_start;
                    Fake_Base_Stations[fake_base_count].x_end = x_end;
                    Fake_Base_Stations[fake_base_count].y_end = y_end;
                    Fake_Base_Stations[fake_base_count].speed = speed;
                    Fake_Base_Stations[fake_base_count].start_hour = start_hour;
                    Fake_Base_Stations[fake_base_count].start_minute = start_minute;
                    Fake_Base_Stations[fake_base_count].num = num;
                    fake_base_count++;
                }
                printf("已成功导入伪基站数据!\n");
                flag = 1;
                fclose(fp);
            }
        }
    }
}

/****** 文件数据的读入功能 ******/

/*----------------------------------------------------------------*/

/****** 文件数据的清除功能 ******/

/*** 清除基站和移动终端数据 ***/
void erase_BS_MT()
{
    base_count = 0;
    mobile_count = 0;
    printf("已清除基站和移动终端的数据！\n");
}

/*** 清除伪基站的数据 ***/
void erase_Fake_BS()
{
    fake_base_count = 0;
    printf("已清除伪基站的数据！\n");
}

/****** 文件数据的清除功能 ******/

/*----------------------------------------------------------------*/

/****** 四叉树的构建功能 ******/

/*** 根据基站数据完成四叉树的生成 ***/
void Build_QuadTree()
{
    if (base_count == 0)
    {
        printf("基站数据为空，请先导入相关数据！\n");
        return;
    }
    printf("四叉树正在构建中...\n");
    get_boundary(Bases, base_count, &up, &bottom, &left, &right); // 首先获取边界值
    root = build_tree(Bases, base_count, NULL, up + 10, bottom - 10, left - 10, right + 10, 0);
    // 其次build_tree实现构造,适当扩大边界，保证囊括所有
    printf("四叉树构建完成!\n");
}

/****** 四叉树的构建功能 ******/

/*----------------------------------------------------------------*/

/****** 主要功能1 ******/

/*** 主要功能1的对外接口 ***/
void main_func1()
{
    char pos[MAX_FILE_LENGTH];
    printf("请输入你要查看的方位(例如：东南、西南): ");
    scanf("%s", pos);
    show_corner(root, pos);
}

/*** 传入四叉树的根结点，和所选方位，展示该方位区域内的所有基站 ***/
void show_corner(QuadTreeNode *node, char pos[])
{
    if (node->is_leaf)
    {
        printf("当区域最大容量为%d时，%s角区域中的基站数据如下:\n", MAX_leaf_BS_COUNT, pos);
        printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\n");
        for (int i = 0; i < node->base_count; i++)
        {
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\n", node->bases[i].num, node->bases[i].type, node->bases[i].power, node->bases[i].x, node->bases[i].y);
        }
        return;
    }
    if (strcmp(pos, "西北") == 0)
        show_corner(node->LU, pos);
    else if (strcmp(pos, "东南") == 0)
        show_corner(node->RB, pos);
    else if (strcmp(pos, "西南") == 0)
        show_corner(node->LB, pos);
    else
        show_corner(node->RU, pos);
}

/****** 主要功能1 ******/

/*----------------------------------------------------------------*/

/****** 主要功能2 ******/
// 显示出西北角分块/树叶的东侧分块/树叶基站数据，以及南侧相邻分块/树叶的基站数据。
// 显示出东南角所在分块/树叶的西北侧基站数据，以及西北侧的北侧相邻分块/树叶的基站数据。
// 共计16分

/*** 主要功能2的对外接口 ***/
void main_func2()
{
    show_LU_east();
    printf("\n");
    show_LU_south();
    printf("\n");
    show_RB_LU();
    printf("\n");
    show_RB_LU_north();
    printf("\n");
}

/*** 显示出西北角分块/树叶的东侧分块/树叶基站数据 ***/
void show_LU_east()
{
    // 首先找到西北角的分块，然后找到其father，往东北走即可
    QuadTreeNode *ptr = root;
    while (!ptr->is_leaf)
        ptr = ptr->LU;
    ptr = ptr->father->RU;
    BASE result[20];      // 用于存储查到的基站
    int result_count = 0; // 用于存储查到的基站数目
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Urban");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Township");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "High_way");
    if (result_count == 0)
        printf("西北角树叶中的东侧树叶无基站数据\n");
    else
    {
        printf("西北角树叶中的东侧树叶的基站数据如下:\n");
        printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\n");
        for (int i = 0; i < result_count; i++)
        {
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\n", result[i].num, result[i].type, result[i].power, result[i].x, result[i].y);
        }
    }
}

/*** 显示出西北角分块/树叶的南侧分块/树叶基站数据 ***/
void show_LU_south()
{
    // 首先找到西北角的分块，然后找到其father，往西南走即可
    QuadTreeNode *ptr = root;
    while (!ptr->is_leaf)
        ptr = ptr->LU;
    ptr = ptr->father->LB;
    BASE result[20];      // 用于存储查到的基站
    int result_count = 0; // 用于存储查到的基站数目
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Urban");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Township");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "High_way");
    if (result_count == 0)
        printf("西北角树叶中的南侧树叶无基站数据\n");
    else
    {
        printf("西北角树叶中的南侧树叶的基站数据如下:\n");
        printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\n");
        for (int i = 0; i < result_count; i++)
        {
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\n", result[i].num, result[i].type, result[i].power, result[i].x, result[i].y);
        }
    }
}

/*** 显示出东南角所在分块/树叶的西北侧基站数据 ***/
void show_RB_LU()
{
    // 首先找到东南角的分块，然后找到其father，往西北走即可
    QuadTreeNode *ptr = root;
    while (!ptr->is_leaf)
        ptr = ptr->RB;
    ptr = ptr->father->LU;
    BASE result[20];      // 用于存储查到的基站
    int result_count = 0; // 用于存储查到的基站数目
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Urban");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Township");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "High_way");
    if (result_count == 0)
        printf("东南角树叶中的西北侧树叶无基站数据\n");
    else
    {
        printf("东南角树叶中的西北侧树叶的基站数据如下:\n");
        printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\n");
        for (int i = 0; i < result_count; i++)
        {
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\n", result[i].num, result[i].type, result[i].power, result[i].x, result[i].y);
        }
    }
}

/*** 显示出东南角所在分块/树叶的西北侧的北侧相邻分块/树叶的基站数据 ***/
void show_RB_LU_north()
{
    // 首先找到东南角的分块，然后找到其father的father，往东北走再往西南走即可
    QuadTreeNode *ptr = root;
    while (!ptr->is_leaf)
        ptr = ptr->RB;
    ptr = ptr->father->father->RU->LB;
    BASE result[20];      // 用于存储查到的基站
    int result_count = 0; // 用于存储查到的基站数目
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Urban");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "Township");
    search_tree(root, ptr->region.up, ptr->region.bottom, ptr->region.left, ptr->region.right, result, &result_count, "High_way");
    if (result_count == 0)
        printf("东南角树叶中的西北侧树叶的北侧相邻树叶无基站数据\n");
    else
    {
        printf("东南角树叶中的西北侧树叶的北侧相邻树叶的基站数据如下:\n");
        printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\n");
        for (int i = 0; i < result_count; i++)
        {
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\n", result[i].num, result[i].type, result[i].power, result[i].x, result[i].y);
        }
    }
}
/****** 主要功能2 ******/

/*----------------------------------------------------------------*/

/****** 主要功能3 ******/

// 实现查找特定坐标的位置的附近最强信号的基站

/*** 主要功能3的对外接口 ***/
void main_func3()
{
    while (true)
    {
        printf("请输入需要查询的位置坐标(输入-1 -1退出): ");
        int x, y;
        scanf("%d %d", &x, &y);
        if (x == -1 && y == -1)
            break;
        BASE *res = find_best_base(x, y);
        if (res == NULL)
            printf("该位置附近无基站!\n");
        else
        {
            double signal_strength = get_signal_strength(res, x, y);
            printf("该坐标处的信号最强基站数据如下:\n");
            printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\t\t\t\t信号强度\n");
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\t\t%lf\n", res->num, res->type, res->power, res->x, res->y,
                   signal_strength);
        }
    }
}

/*** 查询(x,y)处信号最强的基站信息 ***/
BASE *find_best_base(double x, double y)
{
    BASE results[50];
    int result_count = 0;
    get_max_radius(); // 遍历得到不同类型基站的最大有效半径
    // 搜索坐标附近的Urban类型基站
    search_tree(root, y + max_Urban_radius, y - max_Urban_radius, x - max_Urban_radius, x + max_Urban_radius, results, &result_count, "Urban");
    // 搜索坐标附近的Township类型基站
    search_tree(root, y + max_Township_radius, y - max_Township_radius, x - max_Township_radius, x + max_Township_radius, results, &result_count, "Township");
    // 搜索坐标附近的High_way类型基站
    search_tree(root, y + max_High_way_radius, y - max_High_way_radius, x - max_High_way_radius, x + max_High_way_radius, results, &result_count, "High_way");
    if (result_count == 0) // 该坐标处无基站数据
        return NULL;
    else // 该坐标处有基站数据
    {
        BASE *res = results; // 计算信号强度，并取最大的那个
        for (int i = 1; i < result_count; i++)
        {
            if (is_better(results + i, res, x, y) &&
                (get_distance(results[i].x, results[i].y, x, y) < results[i].radius))
                res = results + i;
        }
        if (get_distance(res->x, res->y, x, y) > res->radius)
            return NULL;
        else
            return res;
    }
}

/*** 遍历基站得到不同类型基站的最大有效半径 ***/
void get_max_radius()
{
    // 遍历基站，得到不同类型基站的最大有效半径
    for (int i = 0; i < base_count; i++)
    {
        if (strcmp(Bases[i].type, "Urban") == 0)
        {
            if (Bases[i].radius > max_Urban_radius)
                max_Urban_radius = Bases[i].radius;
        }
        else if (strcmp(Bases[i].type, "Township") == 0)
        {
            if (Bases[i].radius > max_Township_radius)
                max_Township_radius = Bases[i].radius;
        }
        else if (strcmp(Bases[i].type, "High_way") == 0)
        {
            if (Bases[i].radius > max_High_way_radius)
                max_High_way_radius = Bases[i].radius;
        }
    }
}

/*** 判断(x,y)附近的两个基站信号强度的大小关系 ***/
bool is_better(BASE *base1, BASE *base2, double x, double y)
{
    // base1比base2信号强，返回true；否则返回false
    double signal_strength1 = get_signal_strength(base1, x, y);
    double signal_strength2 = get_signal_strength(base2, x, y);
    return signal_strength1 > signal_strength2;
}

/*** 计算一个基站对于x, y处点的信号强度 ***/
double get_signal_strength(BASE *base, double x, double y)
{
    double distance = get_distance(base->x, base->y, x, y);
    return base->power * pow(base->radius / distance, 2);
}

/*** 传入两个点的坐标，计算两点之间的距离 ***/
double get_distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

/****** 主要功能3 ******/

/*----------------------------------------------------------------*/

/****** 主要功能4 ******/

/*** 实现根据终端的移动轨迹分析应该连接的基站时间序列 ***/
void main_func4()
{
    urban_count = 0;
    township_count = 0;
    high_way_count = 0;
    printf("共%d条终端移动轨迹，请输入间隔检查的步长: ", mobile_count);
    scanf("%lf", &step);
    // 遍历所有的终端轨迹数据
    for (int i = 0; i < mobile_count; i++)
    {
        output_result(i);
    }
    printf("在所有轨迹中经过基站的次数统计如下:\n");
    printf("城市基站：\t%d\n", urban_count);
    printf("乡村基站：\t%d\n", township_count);
    printf("高速基站：\t%d\n", high_way_count);
}

/*** 输出第i条终端移动轨迹的基站连接情况 ***/
void output_result(int i)
{
    double distance = get_distance(MobileTerminals[i].x_start, MobileTerminals[i].y_start, MobileTerminals[i].x_end,
                                   MobileTerminals[i].y_end); // 计算起点和终点的距离
    double k = distance / step;                               // 计算终端移动的步数
    double speed = MobileTerminals[i].speed / 3.6;            // 终端移动速度
    double time_span = step / speed;                          // 终端移动的时间间隔
    double dx = 0, dy = 0;
    if (MobileTerminals[i].x_end - MobileTerminals[i].x_start != 0)
        dx = step * (MobileTerminals[i].x_end - MobileTerminals[i].x_start) / distance; // 终端移动步长，x的变化量
    if (MobileTerminals[i].y_end - MobileTerminals[i].y_start != 0)
        dy = step * (MobileTerminals[i].y_end - MobileTerminals[i].y_start) / distance; // 终端移动步长，y的变化量
    printf("当检查的第%d条终端移动轨迹的基站连接情况为:\n", i + 1);
    printf("dist = %.2lf, 步长 = %.2lf 米，时间间隔 = %.2lf, k = %.1lf, dx = %.1lf, dy = %.1lf\n", distance, step, time_span, k, dx, dy);
    int switch_base_count = 0;
    int last_base_id = -1;
    for (int j = 0; j <= k; j++)
    {
        double x = MobileTerminals[i].x_start + j * dx; // 计算终端移动的x坐标
        double y = MobileTerminals[i].y_start + j * dy; // 计算终端移动的y坐标
        BASE *base = find_best_base(x, y);              // 计算信号强度最大的基站

        if (base == NULL || base->num == last_base_id) // 当周围没有基站时继续
            continue;

        int minute = MobileTerminals[i].start_minute + (int)(j * time_span) / 60; // 计算终端移动的时间
        int hour = MobileTerminals[i].start_hour + minute / 60;
        minute = minute % 60;
        double second = j * time_span - (int)(j * time_span / 60) * 60;
        printf("x = %8.1lf, y = %8.1lf, 信号强度最大的基站编号为: %05d, 坐标为: (%6d,%6d), 类型为：%-9s 时间为: %02d:%02d:%05.2lf\n", x, y, base->num, base->x, base->y, base->type, hour, minute, second);
        switch_base_count++;
        if (strcmp(base->type, "Urban") == 0)
            urban_count++;
        else if (strcmp(base->type, "Township") == 0)
            township_count++;
        else if (strcmp(base->type, "High_way") == 0)
            high_way_count++;
        last_base_id = base->num;
    }
    printf("终端移动过程中经过的基站的次数为: %d\n\n", switch_base_count);
}
/****** 主要功能4 ******/

/*----------------------------------------------------------------*/

/****** 查找给定区域内基站 ******/

/*** 实现查找任意区域内的所有基站信息 ***/
void main_func5()
{
    double up1, bottom1, left1, right1;
    printf("请输入查询区域的上下左右边界:\n");
    scanf("%lf%lf%lf%lf", &up1, &bottom1, &left1, &right1);
    // 输出区域内的所有基站信息
    BASE results[MAX_BS_COUNT];
    int result_count = 0;
    search_tree(root, up1, bottom1, left1, right1, results, &result_count, "Urban");
    search_tree(root, up1, bottom1, left1, right1, results, &result_count, "Township");
    search_tree(root, up1, bottom1, left1, right1, results, &result_count, "High_way");
    if (result_count == 0)
        printf("该区域内无基站数据\n");
    else
    {
        printf("该区域内的%d个基站数据如下:\n", result_count);
        printf("基站编号\t\t基站类型\t\t\t基站功率\t\t\t基站坐标\n");
        for (int i = 0; i < result_count; i++)
        {
            printf("%5d\t\t%-10s\t\t%lf\t\t(%d,%d)\n", results[i].num, results[i].type, results[i].power, results[i].x, results[i].y);
        }
    }
}

/****** 查找给定区域内基站 ******/

/*----------------------------------------------------------------*/

/****** 扩展功能1 ******/

void extended_func1()
{
    // 核心算法思想是先找到在这个基站范围内的坐标，然后往左递减和往右递增找到出去的坐标，分别在这两个区间内二分收缩即可
    int line_num = 0;
    int sequence = 0;
    printf("请输入需要分析的线路编号(1～%d): ", mobile_count);
    scanf("%d", &line_num);
    printf("需要查询移动终端经过的第几个基站的有效范围时间: ");
    scanf("%d", &sequence);
    double distance = get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, MobileTerminals[line_num - 1].x_end,
                                   MobileTerminals[line_num - 1].y_end); // 计算起点和终点的距离
    double speed = MobileTerminals[line_num - 1].speed / 3.6;            // 终端移动速度
    double dx = 0, dy = 0;
    if (MobileTerminals[line_num - 1].x_end - MobileTerminals[line_num - 1].x_start != 0)
        dx = step * (MobileTerminals[line_num - 1].x_end - MobileTerminals[line_num - 1].x_start) / distance; // 终端移动步长，x的变化量
    if (MobileTerminals[line_num - 1].y_end - MobileTerminals[line_num - 1].y_start != 0)
        dy = step * (MobileTerminals[line_num - 1].y_end - MobileTerminals[line_num - 1].y_start) / distance; // 终端移动步长，y的变化量
    double pre_x, pre_y, cur_x, cur_y, after_x, after_y;
    BASE *res = NULL;
    int switch_base_count = 0;
    int last_base_id = -1;
    for (int i = 0;; i++)
    {
        double x = MobileTerminals[line_num - 1].x_start + i * dx; // 计算终端移动的x坐标
        double y = MobileTerminals[line_num - 1].y_start + i * dy; // 计算终端移动的y坐标
        BASE *base = find_best_base(x, y);                         // 计算信号强度最大的基站

        if (base == NULL || base->num == last_base_id) // 当周围没有基站时继续
            continue;

        // 保证最初位置的时候的不在首个基站信号范围内
        if (get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, base->x, base->y) > base->radius)
        {
            switch_base_count++;
            last_base_id = base->num;
        }

        if (switch_base_count == sequence)
        {
            res = base;
            pre_x = x, pre_y = y;
            cur_x = x, cur_y = y;
            after_x = x, after_y = y;
            break;
        }
    }
    for (int i = 1;; i++)
    {
        pre_x -= dx * i;
        pre_y -= dy * i;
        if (get_distance(pre_x, pre_y, res->x, res->y) > res->radius)
            break;
    }
    for (int i = 1;; i++)
    {
        after_x += dx * i;
        after_y += dy * i;
        if (get_distance(after_x, after_y, res->x, res->y) > res->radius)
            break;
    }
    printf("%d号基站的断开-连接-断开迭代坐标为: (%.1lf, %.1lf) - (%.1lf, %.1lf) - (%.1lf, %.1lf)\n", res->num, pre_x, pre_y, cur_x, cur_y, after_x, after_y);
    // 二分法求解临界坐标
    double left_x = pre_x, left_y = pre_y, right_x = cur_x, right_y = cur_y;
    double mid_x, mid_y;
    while (get_distance(left_x, left_y, right_x, right_y) > INACCURACY)
    {
        mid_x = (left_x + right_x) / 2;
        mid_y = (left_y + right_y) / 2;
        if (get_distance(mid_x, mid_y, res->x, res->y) < res->radius)
        {
            right_x = mid_x;
            right_y = mid_y;
        }
        else
        {
            left_x = mid_x;
            left_y = mid_y;
        }
    }
    double time = get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, mid_x, mid_y) / speed;
    int minute = MobileTerminals[line_num - 1].start_minute + (int)time / 60;
    int hour = MobileTerminals[line_num - 1].start_hour + minute / 60;
    double second = time - (int)(time / 60) * 60;
    printf("误差为%.2lfm时离开该基站有效范围的临界坐标为: (%.2lf, %.2lf), 时刻为：%02d:%02d:%05.2lf\n", INACCURACY, mid_x, mid_y, hour, minute % 60, second);
    left_x = cur_x, left_y = cur_y, right_x = after_x, right_y = after_y;
    while (get_distance(left_x, left_y, right_x, right_y) > INACCURACY)
    {
        mid_x = (left_x + right_x) / 2;
        mid_y = (left_y + right_y) / 2;
        if (get_distance(mid_x, mid_y, res->x, res->y) < res->radius)
        {
            left_x = mid_x;
            left_y = mid_y;
        }
        else
        {
            right_x = mid_x;
            right_y = mid_y;
        }
    }
    time = get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, mid_x, mid_y) / speed;
    minute = MobileTerminals[line_num - 1].start_minute + (int)time / 60;
    hour = MobileTerminals[line_num - 1].start_hour + minute / 60;
    second = time - (int)(time / 60) * 60;
    printf("误差为%.2lfm时离开该基站有效范围的临界坐标为: (%.2lf, %.2lf), 时刻为：%02d:%02d:%05.4lf\n", INACCURACY, mid_x, mid_y, hour, minute % 60, second);
}

/****** 扩展功能1 ******/

/*----------------------------------------------------------------*/

/****** 扩展功能2 ******/

void extended_func2()
{
    int line_num;
    printf("请输入要分析的轨迹号(3, 6): ");
    scanf("%d", &line_num);
    // 算法核心思想是先找到在两个重叠区域的坐标点，往左递减找到第一个不在重叠区域的坐标点，往右递增找到第一个不在重叠区域的坐标点，然后在这两个坐标点之间进行二分法求解临界坐标
    // 先找到这两个有重复的基站
    BASE *base1 = NULL, *base2 = NULL;
    base1 = find_best_base(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start);
    double distance = get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, MobileTerminals[line_num - 1].x_end,
                                   MobileTerminals[line_num - 1].y_end); // 计算起点和终点的距离
    double dx = 0, dy = 0;
    double speed = MobileTerminals[line_num - 1].speed / 3.6;
    if (MobileTerminals[line_num - 1].x_end - MobileTerminals[line_num - 1].x_start != 0)
        dx = step * (MobileTerminals[line_num - 1].x_end - MobileTerminals[line_num - 1].x_start) / distance; // 终端移动步长，x的变化量
    if (MobileTerminals[line_num - 1].y_end - MobileTerminals[line_num - 1].y_start != 0)
        dy = step * (MobileTerminals[line_num - 1].y_end - MobileTerminals[line_num - 1].y_start) / distance; // 终端移动步长，y的变化量
    double pre_x, pre_y, cur_x, cur_y, after_x, after_y;
    // 递增找到切换位置的坐标点，成功找到两个不同的基站
    for (int i = 1;; i++)
    {
        cur_x = MobileTerminals[line_num - 1].x_start + dx * i;
        cur_y = MobileTerminals[line_num - 1].y_start + dy * i;
        base2 = find_best_base(cur_x, cur_y);
        if (base1->num != base2->num)
            break;
    }
    // 如果一开始就在两个基站的重叠区域，那么应该找到下一个基站的重叠区域
    if (get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, base1->x, base1->y) < base1->radius &&
        get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, base2->x, base2->y) < base2->radius)
    {
        base1 = base2;
        for (int i = 1;; i++)
        {
            cur_x += dx, cur_y += dy;
            base2 = find_best_base(cur_x, cur_y);
            if (base1->num != base2->num)
                break;
        }
    }
    else
    {
        BASE *tmp = base1;
        base1 = base2;
        base2 = tmp;
    }
    printf("信号由第一个基站切换到第二个基站：\n第一个基站编号为: %d, 坐标为: (%d, %d)\n第二个基站编号为: %d, 坐标为: (%d, %d)\n", base1->num, base1->x, base1->y, base2->num, base2->x, base2->y);
    if (base2 != base1)
    {
        for (int i = 1;; i++)
        {
            cur_x = MobileTerminals[line_num - 1].x_start + dx * i;
            cur_y = MobileTerminals[line_num - 1].y_start + dy * i;
            if (get_distance(cur_x, cur_y, base1->x, base1->y) < base1->radius && get_distance(cur_x, cur_y, base2->x, base2->y) < base2->radius)
                break;
        }
        for (int i = 0;; i++)
        {
            after_x = cur_x + dx * (i + 1);
            after_y = cur_y + dy * (i + 1);
            if (get_distance(after_x, after_y, base1->x, base1->y) > base1->radius)
                break;
        }
        for (int i = 0;; i++)
        {
            pre_x = cur_x - dx * (i + 1);
            pre_y = cur_y - dy * (i + 1);
            if (get_distance(pre_x, pre_y, base2->x, base2->y) > base2->radius)
                break;
        }
        if (dx < 0 && pre_x > MobileTerminals[line_num - 1].x_start || dx > 0 && pre_x < MobileTerminals[line_num - 1].x_start)
            pre_x = MobileTerminals[line_num - 1].x_start;
        if (dy < 0 && pre_y > MobileTerminals[line_num - 1].y_start || dy > 0 && pre_y < MobileTerminals[line_num - 1].y_start)
            pre_y = MobileTerminals[line_num - 1].y_start;
        printf("不在重叠区域-在重叠区域-不在重叠区域的迭代坐标为(%.1lf, %.1lf)-(%.1lf, %.1lf)-(%.1lf, %.1lf)\n", pre_x, pre_y, cur_x, cur_y, after_x, after_y);
        // 二分法求解临界坐标
        double left_x, left_y, right_x, right_y, mid_x, mid_y;
        left_x = pre_x;
        left_y = pre_y;
        right_x = cur_x;
        right_y = cur_y;
        while (get_distance(left_x, left_y, right_x, right_y) > INACCURACY)
        {
            mid_x = (left_x + right_x) / 2;
            mid_y = (left_y + right_y) / 2;
            if (get_distance(mid_x, mid_y, base1->x, base1->y) < base1->radius && get_distance(mid_x, mid_y, base2->x, base2->y) < base2->radius)
            {
                right_x = mid_x;
                right_y = mid_y;
            }
            else
            {
                left_x = mid_x;
                left_y = mid_y;
            }
        }
        double time1 = get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, mid_x, mid_y) / speed;
        int minute1 = MobileTerminals[line_num - 1].start_minute + (int)time1 / 60;
        int hour1 = MobileTerminals[line_num - 1].start_hour + minute1 / 60;
        double second1 = time1 - (int)(time1 / 60) * 60;
        printf("误差为%.2lf时进入重叠区域的临界坐标为: (%.2lf, %.2lf), 时刻为：%02d:%02d:%05.2lf\n", INACCURACY, mid_x, mid_y, hour1, minute1 % 60, second1);
        // 二分法求解临界坐标
        left_x = cur_x;
        left_y = cur_y;
        right_x = after_x;
        right_y = after_y;
        while (get_distance(left_x, left_y, right_x, right_y) > INACCURACY)
        {
            mid_x = (left_x + right_x) / 2;
            mid_y = (left_y + right_y) / 2;
            if (get_distance(mid_x, mid_y, base1->x, base1->y) < base1->radius && get_distance(mid_x, mid_y, base2->x, base2->y) < base2->radius)
            {
                left_x = mid_x;
                left_y = mid_y;
            }
            else
            {
                right_x = mid_x;
                right_y = mid_y;
            }
        }
        double time2 = get_distance(MobileTerminals[line_num - 1].x_start, MobileTerminals[line_num - 1].y_start, mid_x, mid_y) / speed;
        int minute2 = MobileTerminals[line_num - 1].start_minute + (int)time2 / 60;
        int hour2 = MobileTerminals[line_num - 1].start_hour + minute2 / 60;
        double second2 = time2 - (int)(time2 / 60) * 60;
        printf("误差为%.2lf时进入重叠区域的临界坐标为: (%.2lf, %.2lf), 时刻为：%02d:%02d:%05.2lf\n", INACCURACY, mid_x, mid_y, hour2, minute2 % 60, second2);
        printf("进入重叠区域的持续时间为 time2 - time1 = %.2f - %.2f = %.2f s\n", time2, time1, time2 - time1);
    }
}
/****** 扩展功能2 ******/

/*----------------------------------------------------------------*/

/****** 升级功能 ******/

void upgrade_func()
{
    printf("请输入要分析的轨迹编号(1 - %d): ", mobile_count);
    int line_num;
    scanf("%d", &line_num);
    double MT_x_start, MT_y_start, MT_x_end, MT_y_end;
    double MT_start_time, MT_end_time;
    double Fake_x_start, Fake_y_start, Fake_x_end, Fake_y_end;
    double Fake_start_time, Fake_end_time;
    double MT_speed, Fake_speed;
    double MT_dx, MT_dy, Fake_dx, Fake_dy;
    int flag = 0;
    for (int i = 0; i < fake_base_count; i++)
    {
        // 首先应该把他们调到同一个时间点
        MT_x_start = MobileTerminals[line_num - 1].x_start;
        MT_y_start = MobileTerminals[line_num - 1].y_start;
        MT_x_end = MobileTerminals[line_num - 1].x_end;
        MT_y_end = MobileTerminals[line_num - 1].y_end;
        MT_speed = MobileTerminals[line_num - 1].speed / 3.6;
        MT_start_time = MobileTerminals[line_num - 1].start_hour * 3600 + MobileTerminals[line_num - 1].start_minute * 60;
        MT_end_time = MT_start_time + get_distance(MT_x_start, MT_y_start, MT_x_end, MT_y_end) / MT_speed;
        MT_dx = (MT_x_end - MT_x_start) / (get_distance(MT_x_start, MT_y_start, MT_x_end, MT_y_end));
        MT_dy = (MT_y_end - MT_y_start) / (get_distance(MT_x_start, MT_y_start, MT_x_end, MT_y_end));

        Fake_x_start = Fake_Base_Stations[i].x_start;
        Fake_y_start = Fake_Base_Stations[i].y_start;
        Fake_x_end = Fake_Base_Stations[i].x_end;
        Fake_y_end = Fake_Base_Stations[i].y_end;
        Fake_speed = Fake_Base_Stations[i].speed / 3.6;
        Fake_start_time = Fake_Base_Stations[i].start_hour * 3600 + Fake_Base_Stations[i].start_minute * 60;
        Fake_end_time = Fake_start_time + get_distance(Fake_x_start, Fake_y_start, Fake_x_end, Fake_y_end) / Fake_speed;
        Fake_dx = (Fake_x_end - Fake_x_start) /
                  (get_distance(Fake_x_start, Fake_y_start, Fake_x_end, Fake_y_end));
        Fake_dy = (Fake_y_end - Fake_y_start) /
                  (get_distance(Fake_x_start, Fake_y_start, Fake_x_end, Fake_y_end));

        if (Fake_end_time < MT_start_time || Fake_start_time > MT_end_time)
            continue;

        if (Fake_start_time < MT_start_time)
        {
            Fake_x_start = Fake_x_start + (MT_start_time - Fake_start_time) * Fake_dx * Fake_speed;
            Fake_y_start = Fake_y_start + (MT_start_time - Fake_start_time) * Fake_dy * Fake_speed;
            Fake_start_time = MT_start_time;
        }
        else if (Fake_start_time > MT_start_time)
        {
            MT_x_start = MT_x_start + (Fake_start_time - MT_start_time) * MT_dx * MT_speed;
            MT_y_start = MT_y_start + (Fake_start_time - MT_start_time) * MT_dy * MT_speed;
            MT_start_time = Fake_start_time;
        }

        // 计算出时间间隔的次数
        int time_span_counts = (int)((MT_end_time - MT_start_time) / TIME_STEP);
        if ((int)((MT_end_time - MT_start_time) / TIME_STEP > (int)(Fake_end_time - Fake_start_time) / TIME_STEP))
            time_span_counts = (int)(Fake_end_time - Fake_start_time / TIME_STEP);

        int j; // 用于表示经历了多少时间间隔
        for (j = 0; j <= time_span_counts; j++)
        {
            if (get_distance(MT_x_start + j * TIME_STEP * MT_dx * MT_speed, MT_y_start + j * TIME_STEP * MT_dy * MT_speed, Fake_x_start + j * TIME_STEP * Fake_dx * Fake_speed, Fake_y_start + j * TIME_STEP * Fake_dy * Fake_speed) < 40)
                break;
        }
        if (j > time_span_counts)
            continue;

        flag = 1;
        // 说明在这个时间间隔内发生了信号的连接
        printf("连接上的伪基站编号是%d\n", Fake_Base_Stations[i].num);
        double MT_cur_x, MT_cur_y, Fake_cur_x, Fake_cur_y;
        double MT_prev_x, MT_prev_y, Fake_prev_x, Fake_prev_y;
        double MT_next_x, MT_next_y, Fake_next_x, Fake_next_y;
        MT_cur_x = MT_x_start + j * TIME_STEP * MT_dx * MT_speed;
        MT_cur_y = MT_y_start + j * TIME_STEP * MT_dy * MT_speed;
        Fake_cur_x = Fake_x_start + j * TIME_STEP * Fake_dx * Fake_speed;
        Fake_cur_y = Fake_y_start + j * TIME_STEP * Fake_dy * Fake_speed;

        for (int k = 1; k <= j; k++)
        {
            MT_prev_x = MT_cur_x - TIME_STEP * MT_dx * MT_speed * k;
            MT_prev_y = MT_cur_y - TIME_STEP * MT_dy * MT_speed * k;
            Fake_prev_x = Fake_cur_x - TIME_STEP * Fake_dx * Fake_speed * k;
            Fake_prev_y = Fake_cur_y - TIME_STEP * Fake_dy * Fake_speed * k;
            if (get_distance(MT_prev_x, MT_prev_y, Fake_prev_x, Fake_prev_y) > 40)
                break;
        }

        for (int k = 1; k <= time_span_counts - j; k++)
        {
            MT_next_x = MT_cur_x + TIME_STEP * MT_dx * MT_speed * k;
            MT_next_y = MT_cur_y + TIME_STEP * MT_dy * MT_speed * k;
            Fake_next_x = Fake_cur_x + TIME_STEP * Fake_dx * Fake_speed * k;
            Fake_next_y = Fake_cur_y + TIME_STEP * Fake_dy * Fake_speed * k;
            if (get_distance(MT_next_x, MT_next_y, Fake_next_x, Fake_next_y) > 40)
                break;
        }
        // 二分法查找40米的临界点
        double MT_mid_x, MT_mid_y, Fake_mid_x, Fake_mid_y;
        double MT_left_x = MT_prev_x, MT_right_x = MT_cur_x, MT_left_y = MT_prev_y, MT_right_y = MT_cur_y;
        double Fake_left_x = Fake_prev_x, Fake_right_x = Fake_cur_x, Fake_left_y = Fake_prev_y, Fake_right_y = Fake_cur_y;
        while (true)
        {
            MT_mid_x = (MT_left_x + MT_right_x) / 2;
            MT_mid_y = (MT_left_y + MT_right_y) / 2;
            Fake_mid_x = (Fake_left_x + Fake_right_x) / 2;
            Fake_mid_y = (Fake_left_y + Fake_right_y) / 2;
            double distance = get_distance(MT_mid_x, MT_mid_y, Fake_mid_x, Fake_mid_y);
            if (distance < INACCURACY + 40 && distance > 40 - INACCURACY)
                break;
            if (get_distance(MT_mid_x, MT_mid_y, Fake_mid_x, Fake_mid_y) > 40)
            {
                MT_left_x = MT_mid_x;
                MT_left_y = MT_mid_y;
                Fake_left_x = Fake_mid_x;
                Fake_left_y = Fake_mid_y;
            }
            else
            {
                MT_right_x = MT_mid_x;
                MT_right_y = MT_mid_y;
                Fake_right_x = Fake_mid_x;
                Fake_right_y = Fake_mid_y;
            }
        }
        double time1 = MT_start_time + (MT_mid_x - MT_x_start) / (MT_speed * MT_dx);
        int hours1 = (int)(time1 / 3600);
        int minutes1 = (int)((time1 - hours1 * 3600) / 60);
        double seconds1 = time1 - hours1 * 3600 - minutes1 * 60;
        printf("移动终端连接上伪基站的时刻为 %02d:%02d:%.2lf\n", hours1, minutes1, seconds1);
        printf("移动终端临界点坐标为：(%.2lf, %.2lf)\n", MT_mid_x, MT_mid_y);
        printf("伪基站临界点坐标为：(%.2lf, %.2lf)\n", Fake_mid_x, Fake_mid_y);
        // 二分法查找40米的临界点
        MT_left_x = MT_cur_x, MT_right_x = MT_next_x, MT_left_y = MT_cur_y, MT_right_y = MT_next_y;
        Fake_left_x = Fake_cur_x, Fake_right_x = Fake_next_x, Fake_left_y = Fake_cur_y, Fake_right_y = Fake_next_y;
        while (true)
        {
            MT_mid_x = (MT_left_x + MT_right_x) / 2;
            MT_mid_y = (MT_left_y + MT_right_y) / 2;
            Fake_mid_x = (Fake_left_x + Fake_right_x) / 2;
            Fake_mid_y = (Fake_left_y + Fake_right_y) / 2;
            double distance = get_distance(MT_mid_x, MT_mid_y, Fake_mid_x, Fake_mid_y);
            if (distance < INACCURACY + 40 && distance > 40 - INACCURACY)
                break;
            if (get_distance(MT_mid_x, MT_mid_y, Fake_mid_x, Fake_mid_y) > 40)
            {
                MT_right_x = MT_mid_x;
                MT_right_y = MT_mid_y;
                Fake_right_x = Fake_mid_x;
                Fake_right_y = Fake_mid_y;
            }
            else
            {
                MT_left_x = MT_mid_x;
                MT_left_y = MT_mid_y;
                Fake_left_x = Fake_mid_x;
                Fake_left_y = Fake_mid_y;
            }
        }
        double time2 = MT_start_time + (MT_mid_x - MT_x_start) / (MT_speed * MT_dx);
        int hours2 = (int)(time2 / 3600);
        int minutes2 = (int)((time2 - hours2 * 3600) / 60);
        double seconds2 = time2 - hours2 * 3600 - minutes2 * 60;
        printf("移动终端断开与伪基站的时刻为 %02d:%02d:%.2lf\n", hours2, minutes2, seconds2);
        printf("移动终端临界点坐标为：(%.2lf, %.2lf)\n", MT_mid_x, MT_mid_y);
        printf("伪基站临界点坐标为：(%.2lf, %.2lf)\n", Fake_mid_x, Fake_mid_y);
        printf("移动终端与伪基站连接的时间为：%.2lf秒\n", time2 - time1);
    }
    if (flag == 0)
        printf("该轨迹的移动终端与伪基站没有连接!\n");
}

/****** 升级功能 ******/

/*----------------------------------------------------------------*/
