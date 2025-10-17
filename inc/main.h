#ifndef __MAIN_H_
#define __MAIN_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <math.h>
#include <linux/input.h>
#include <stdbool.h>
#include "kernel_list.h"
// #include "show_bmp.c"
#include "font.h"
// #include "login.c"
#include <unistd.h>
// #include "picture_down.c"
// #include "Determine_dire.c"

#define RED 0x00ff0000    // 红色0
#define GREEN 0x0000ff00  // 绿色1
#define BLUE 0x000000ff   // 蓝色2
#define YELLOW 0x00ffff00 // 黄色3
#define BLACK 0x00000000  // 黑色4
#define WHITE 0x00ffffff  // 白色5
#define PURPLE 0x00800080 // 紫色
#define test 0x0000ffff   // 测试


// 函数声明
void draw_ret_color(int (*lcd_mmap)[800]);
int get_direction();
void register_user(const char *username, const char *password);
int login_user(const char *username, const char *password);
void appendArrayToFile(int (*lcd_mmap)[800], const char *buf_ID);
void readArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID);
void read_tail_ArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID);
void read_next_ArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID, int *time_go);
void read_pre_ArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID, int *time_go);
int show_bmp(const char *bmp_file_name, int x, int y, int (*lcd_mmap)[800]);

typedef struct ElemNode // 撤回节点的结构体
{
    int (*last)[700];
    struct list_head list;
} ElemNode;

typedef struct Lhead // 撤回链表的头指针
{
    int num, num_tail;
    struct list_head *list_tail;
    struct list_head list;
} Lhead;

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

// #define INIT_LIST_HEAD(ptr)

#endif