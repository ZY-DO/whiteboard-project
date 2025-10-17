#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 用于 sleep 函数

#define FILENAME "./data/user_data.txt"
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100

// 注册函数，接收用户名和密码作为参数
void register_user(const char *username, const char *password)
{
    FILE *file;
    char line[256];
    int user_exists = 0;

    // 检查是否有重复用户名
    file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("无法打开文件。\n");
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        char existing_username[128];
        sscanf(line, "%s", existing_username);

        if (strcmp(existing_username, username) == 0)
        {
            user_exists = 1;
            break;
        }
    }
    fclose(file);

    if (user_exists)
    {
        // 用户名已存在
        font_login_more(220, 280, "已有用户名！", 30);
    sleep(1);
    font_login_more(220, 280, "", 30);
        return;
    }

    // 如果没有重复用户名，则进行注册
    file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        printf("无法打开文件。\n");
        return;
    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    // 提示注册成功
    font_login_more(220, 280, "注册成功！", 30);
    sleep(1);
    font_login_more(220, 280, "", 30);
}


// 登录函数，接收用户名和密码作为参数
int login_user(const char *username, const char *password)
{
    char file_username[MAX_USERNAME_LENGTH];
    char file_password[MAX_PASSWORD_LENGTH];
    FILE *file;
    int found = 0;

    file = fopen(FILENAME, "r+");
    if (file == NULL)
    {
        font_login_more(220, 280, "此账号无注册!", 30);
        sleep(1);
        font_login_more(220, 280, "", 30);
        return 0;
    }

    while (fscanf(file, "%s %s", file_username, file_password) != EOF)
    {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0)
        {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found)
    {
        font_login_more(220, 280, "登录成功！", 30);
        sleep(1);
        font_login_more(220, 280, "", 30);
        return 1;
    }
    else
    {
        font_login_more(220, 280, "用户名或密码错误！", 30);
        sleep(1);
        font_login_more(220, 280, "", 30);
        return 0;
    }
}

