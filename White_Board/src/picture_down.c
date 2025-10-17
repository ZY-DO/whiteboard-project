#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 追加写入数据到文件（以二进制形式）
void appendArrayToFile(int (*lcd_mmap)[800], const char *buf_ID)
{
    // 确保 buf_ID 不超过 19 个字符
    if (buf_ID == NULL || strlen(buf_ID) >= 20)
    {
        printf("用户名无效。\n");
        return;
    }

    // 构造文件名
    char filename[40];
    snprintf(filename, sizeof(filename), "./data/用户%s.txt", buf_ID);

    // 以追加模式打开文件
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行追加写入。\n", filename);
        return;
    }

    int lcd_cpy[380][700];
    // 写入数据
    for (int i = 0; i < 380; ++i)
    {
        for (int j = 0; j < 700; ++j)
        {
            lcd_cpy[i][j] = lcd_mmap[i+100][j+100];
        }
    }
    int ret = fwrite(lcd_cpy, 4, 380*700, file);
    printf("%d\n", ret);
    // 关闭文件
    fclose(file);

    printf("数据成功追加到 %s\n", filename);
}

// 读取文件中的数据（以二进制形式）
void readArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID)
{
    // 确保 buf_ID 不超过 19 个字符
    if (buf_ID == NULL || strlen(buf_ID) >= 20)
    {
        printf("用户名无效。\n");
        return;
    }

    // 构造文件名
    char filename[40];
    snprintf(filename, sizeof(filename), "./data/用户%s.txt", buf_ID);

    // 打开文件
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行读取。\n", filename);
        return;
    }
    // fseek(file, 2128000, SEEK_SET);
    // 读取数据
    int lcd_cpy[380][700];
    int ret = fread(lcd_cpy, 4, 700 * 380, file);
    if (ret != (700 * 380))
    {
        printf("文件格式错误或数据不完整。\n");
        printf("%d", ret);
        fclose(file);
        return;
    }
    for (int i = 0; i < 380; ++i)
    {
        for (int j = 0; j < 700; ++j)
        {
            lcd_mmap[i+100][j+100] = lcd_cpy[i][j];
        }
    }

    // 关闭文件
    fclose(file);

    printf("数据成功读取自 %s\n", filename);
}

// 读取最后一个画册
void read_tail_ArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID)
{
    // 确保 buf_ID 不超过 19 个字符
    if (buf_ID == NULL || strlen(buf_ID) >= 20)
    {
        printf("用户名无效。\n");
        return;
    }

    // 构造文件名
    char filename[40];
    snprintf(filename, sizeof(filename), "./data/用户%s.txt", buf_ID);

    // 打开文件
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行读取。\n", filename);
        return;
    }
    fseek(file, -1064000, SEEK_END);
    // 读取数据
    int lcd_cpy[380][700];
    int ret = fread(lcd_cpy, 4, 700 * 380, file);
    if (ret != (700 * 380))
    {
        printf("文件格式错误或数据不完整。\n");
        printf("%d", ret);
        fclose(file);
        return;
    }
    for (int i = 0; i < 380; ++i)
    {
        for (int j = 0; j < 700; ++j)
        {
            lcd_mmap[i+100][j+100] = lcd_cpy[i][j];
        }
    }

    // 关闭文件
    fclose(file);

    printf("数据成功读取自 %s\n", filename);
}

// 相册前进模式
void read_next_ArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID, int *time_go)
{
    // 确保 buf_ID 不超过 19 个字符
    if (buf_ID == NULL || strlen(buf_ID) >= 20)
    {
        printf("用户名无效。\n");
        return;
    }

    // 构造文件名
    char filename[40];
    snprintf(filename, sizeof(filename), "./data/用户%s.txt", buf_ID);

    // 打开文件
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行读取。\n", filename);
        return;
    }
    fseek(file, 1064000 * (*time_go), SEEK_SET);
    // 读取数据
   int lcd_cpy[380][700];
    int ret = fread(lcd_cpy, 4, 700 * 380, file);
    if (ret != (700 * 380))
    {
        (*time_go)--;
        printf("文件格式错误或数据不完整。\n");
        printf("%d", ret);
        fclose(file);
        return;
    }
    for (int i = 0; i < 380; ++i)
    {
        for (int j = 0; j < 700; ++j)
        {
            lcd_mmap[i+100][j+100] = lcd_cpy[i][j];
        }
    }

    // 关闭文件
    fclose(file);

    printf("数据成功读取自 %s\n", filename);
}

// 相册后退模式
void read_pre_ArrayFromFile(int (*lcd_mmap)[800], const char *buf_ID, int *time_go)
{
    // 确保 buf_ID 不超过 19 个字符
    if (buf_ID == NULL || strlen(buf_ID) >= 20)
    {
        printf("用户名无效。\n");
        return;
    }

    // 构造文件名
    char filename[40];
    snprintf(filename, sizeof(filename), "./data/用户%s.txt", buf_ID);

    // 打开文件
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行读取。\n", filename);
        return;
    }
    fseek(file, 1064000 * (*time_go), SEEK_SET);
    int lcd_cpy[380][700];
    int ret = fread(lcd_cpy, 4, 700 * 380, file);
    if (ret != (700 * 380))
    {
        (*time_go)++;
        printf("文件格式错误或数据不完整。\n");
        printf("%d", ret);
        fclose(file);
        return;
    }
    if (*time_go <= 0)
        *time_go = 0;

    for (int i = 0; i < 380; ++i)
    {
        for (int j = 0; j < 700; ++j)
        {
            lcd_mmap[i+100][j+100] = lcd_cpy[i][j];
        }
    }

    // 关闭文件
    fclose(file);

    printf("数据成功读取自 %s\n", filename);
}