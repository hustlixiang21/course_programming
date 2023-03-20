#include "function.h"
#include <unistd.h>

char first_menu_select = 0;  // 记录第一级菜单的选择
char second_menu_select = 0; // 记录第二级菜单的选择

/****** 菜单的总控函数 ******/
void menu_init();

/****** 展示第一级菜单 ******/
void first_show_menu();

/****** 展示一级菜单输入数字1时的第二级菜单 ******/
void second_show_menu1();

/****** 一级菜单输入数字1时的第二级菜单控制函数 ******/
void menu_init1();

/****** 展示一级菜单输入数字3时的第二级菜单 ******/
void second_show_menu2();

/****** 一级菜单输入数字3时的第二级菜单控制函数 ******/
void menu_init2();

/****** 展示一级菜单输入数字4时的第二级菜单 ******/
void second_show_menu3();

/****** 一级菜单输入数字4时的第二级菜单控制函数 ******/
void menu_init3();


int main()
{
    menu_init();
    return 0;
}

/****** 菜单的总控函数 ******/
void menu_init()
{
    first_show_menu();
    // 根据一级菜单的数据选择进行条件判断
    if (first_menu_select == '1')
        menu_init1();
    else if (first_menu_select == '2')
    {
        Build_QuadTree();
        menu_init(); // 生成四叉树的函数的接口处
    }
    else if (first_menu_select == '3')
        menu_init2();
    else if (first_menu_select == '4')
        menu_init3();
    else
        printf("成功退出！\n");
    exit(0);
}

/****** 展示第一级菜单 ******/
void first_show_menu()
{
    while (1)
    {
        // system("clear");
        printf(
            "--------------------------\n"
            "      输入数字选择功能       \n"
            "--------------------------\n"
            "    [1] 导入相关数据信息     \n"
            "    [2] 生成相应的四叉树     \n"
            "    [3] 选择所使用的功能     \n"
            "    [4] 清除已导入的数据     \n"
            "    [5] 退出               \n"
            "--------------------------\n");
        first_menu_select = getchar();
        while (getchar() != '\n')
            continue;
        if (first_menu_select < '1' || first_menu_select > '5')
        {
            printf("--------------------------\n"
                   "      输入有误 再试一次      \n"
                   "--------------------------\n");
            usleep(300 * 1000); // 让屏幕sleep一段时间
            continue;
        }
        else
            break;
    }
}

/****** 展示一级菜单输入数字1时的第二级菜单 ******/
void second_show_menu1()
{
    while (1)
    {
        // system("clear");
        printf(
            "--------------------------\n"
            "      输入数字选择功能       \n"
            "--------------------------\n"
            "    [1] 导入基站与终端数据   \n"
            "    [2] 导入伪基站数据       \n"
            "    [3] 返回上一级菜单       \n"
            "    [4] 退出               \n"
            "--------------------------\n");
        second_menu_select = getchar();
        while (getchar() != '\n')
            continue;
        if (second_menu_select < '1' || second_menu_select > '4')
        {
            printf("--------------------------\n"
                   "      输入有误 再试一次      \n"
                   "--------------------------\n");
            usleep(300 * 1000);
            continue;
        }
        else
            break;
    }
}

/****** 一级菜单输入数字1时的第二级菜单控制函数 ******/
void menu_init1()
{
    second_show_menu1();
    if (second_menu_select == '1')
    {
        BS_MT_Input();
        fflush(stdin);
        menu_init1();
    }
    else if (second_menu_select == '2')
    {
        FAKE_BS_file_input();
        fflush(stdin);
        menu_init1();
    }
    else if (second_menu_select == '3')
    {
        menu_init();
    }
    else
        printf("成功退出！\n");
    exit(0);
}

/****** 展示一级菜单输入数字3时的第二级菜单 ******/
void second_show_menu2()
{
    while (1)
    {
        // system("clear");
        printf(
            "--------------------------\n"
            "      输入数字选择功能       \n"
            "--------------------------\n"
            "       [1] 主要功能1        \n"
            "       [2] 主要功能2        \n"
            "       [3] 主要功能3        \n"
            "       [4] 主要功能4        \n"
            "       [5] 给定区域内基站    \n"
            "       [6] 扩展功能1        \n"
            "       [7] 扩展功能2        \n"
            "       [8] 升级功能         \n"
            "       [9] 返回上一级菜单    \n"
            "       [q] 退出本程序       \n"
            "--------------------------\n");
        second_menu_select = getchar();
        while (getchar() != '\n')
            continue;
        if (second_menu_select < '1' || second_menu_select > '9' && second_menu_select != 'q')
        {
            printf("--------------------------\n"
                   "      输入有误 再试一次      \n"
                   "--------------------------\n");
            usleep(300 * 1000);
            continue;
        }
        else
            break;
    }
}

/****** 一级菜单输入数字2时的第二级菜单控制函数 ******/
void menu_init2()
{
    second_show_menu2();
    if (second_menu_select == '1') // 主要功能1
    {
        main_func1();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '2') // 主要功能2
    {
        main_func2();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '3') // 主要功能3
    {
        main_func3();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '4') // 主要功能4
    {
        main_func4();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '5') // 给定区域内基站
    {
        main_func5();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '6') // 扩展功能1
    {
        extended_func1();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '7') // 扩展功能2
    {
        extended_func2();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '8') // 升级功能1
    {
        upgrade_func();
        fflush(stdin);
        menu_init2();
    }
    else if (second_menu_select == '9') // 返回上一级菜单
    {
        menu_init();
    }
    else
        printf("成功退出！\n");
    exit(0);
}

/****** 展示一级菜单输入数字1时的第二级菜单 ******/
void second_show_menu3()
{
    while (1)
    {
        // system("clear");
        printf(
                "--------------------------\n"
                "      输入数字选择功能       \n"
                "--------------------------\n"
                "    [1] 清除基站和终端数据   \n"
                "    [2] 清除伪基站数据      \n"
                "    [3] 返回上一级菜单       \n"
                "    [4] 退出               \n"
                "--------------------------\n");
        second_menu_select = getchar();
        while (getchar() != '\n')
            continue;
        if (second_menu_select < '1' || second_menu_select > '4')
        {
            printf("--------------------------\n"
                   "      输入有误 再试一次      \n"
                   "--------------------------\n");
            usleep(300 * 1000);
            continue;
        }
        else
            break;
    }
}

/****** 一级菜单输入数字1时的第二级菜单控制函数 ******/
void menu_init3()
{
    second_show_menu3();
    if (second_menu_select == '1')
    {
        erase_BS_MT();
        fflush(stdin);
        menu_init3();
    }
    else if (second_menu_select == '2')
    {
        erase_Fake_BS();
        fflush(stdin);
        menu_init3();
    }
    else if (second_menu_select == '3')
    {
        menu_init();
    }
    else
        printf("成功退出！\n");
    exit(0);
}
