#include "main.h"

int color[7] = {RED, GREEN, BLUE, YELLOW, BLACK, WHITE};
int cr = 0;       // 颜色
int rd = 20;      // 半径
int vcol = 2;     // 滑动变色变量
bool stop = 0;    // 防误触
int touch_my = 0; // 解决  按下松开都要在那块位置
int while_times = 0, while_times1 = 0;
int withfor = 0; //若在回撤中滑动了，则禁止前进

int logdraw = 0;

// 登录信息
char buf_ID[20] = {0}; // 登录界面按键存档
int buf_ID_len = 0;
char buf_PS[20] = {0}; // 登录界面按键存档
int buf_PS_len = 0;

// 初始化左边栏，从属于draw_face
void draw_left(size_t top, size_t bottom, int (*lcd_mmap)[800], int color)
{
    if (color == GREEN)

        for (size_t row = top; row < bottom; row++)
        {
            for (size_t col = 0; col < 89; col++)
            {
                lcd_mmap[row][col] = color;
            }
        }
    else
        for (size_t row = top; row < bottom; row++)
        {
            for (size_t col = 0; col < 90; col++)
            {
                lcd_mmap[row][col] = color;
            }
        }
}

// 初始化主面板
void draw_face(int (*lcd_mmap)[800])
{

    // 白边
    {
        for (size_t row = 90; row < 100; row++) // 上横线
        {
            for (size_t col = 0; col < 800; col++)
            {
                if (row == 90 || row == 99)
                    lcd_mmap[row][col] = BLACK;
                else
                    lcd_mmap[row][col] = WHITE;
            }
        }
        for (size_t col = 90; col < 100; col++) // 左竖线
        {
            for (size_t row = 0; row < 480; row++)
            {
                if (row >= 0 && row < 90)
                {
                    if ((col == 90 || col == 99))
                        lcd_mmap[row][col - 1] = BLACK;
                    else
                        lcd_mmap[row][col - 1] = WHITE;
                }
                else
                {
                    if ((col == 90 || col == 99) && (row < 90 || row > 99) && (row < 200 || row >= 300))
                        lcd_mmap[row][col] = BLACK;
                    else if ((col == 90 || col == 99) && (row >= 200 && row < 300)) // 修改绿色偏移问题
                        lcd_mmap[row][col - 1] = BLACK;
                    else if ((90 < col && col < 99) && (row < 200 || row >= 300))
                        lcd_mmap[row][col] = WHITE;
                    else if ((90 < col && col < 99) && (row >= 200 && row < 300)) // 修改绿色偏移问题,黑色会显示白边问题依然未解决
                        lcd_mmap[row][col - 1] = WHITE;
                }
            }
        }
        for (size_t row = 200; row < 300; row++)
        {
            lcd_mmap[row][99] = WHITE;
        }

        for (size_t col = 369; col < 380; col++) // 右竖线-半径圆
        {
            for (size_t row = 0; row < 90; row++)
            {
                if (col == 369 || col == 379)
                    lcd_mmap[row][col] = BLACK;
                else
                    lcd_mmap[row][col] = WHITE;
            }
        }

        for (size_t col = 471; col <= 480; col++) // 右竖线-撤回
        {
            for (size_t row = 0; row < 90; row++)
            {
                if (col == 471 || col == 480)
                    lcd_mmap[row][col] = BLACK;
                else
                    lcd_mmap[row][col] = WHITE;
            }
        }

        for (size_t col = 631; col <= 640; col++) // 右竖线-保存
        {
            for (size_t row = 0; row < 90; row++)
            {
                if (col == 631 || col == 640)
                    lcd_mmap[row][col] = BLACK;
                else
                    lcd_mmap[row][col] = WHITE;
            }
        }

        for (size_t col = 726; col <= 735; col++) // 右竖线-退出
        {
            for (size_t row = 0; row < 90; row++)
            {
                if (col == 726 || col == 735)
                    lcd_mmap[row][col] = BLACK;
                else
                    lcd_mmap[row][col] = WHITE;
            }
        }
    }
    // 左边栏
    {
        draw_left(100, 200, lcd_mmap, RED);      // 红色
        draw_left(200, 300, lcd_mmap, GREEN);    // 绿色
        draw_left(300, 400, lcd_mmap, BLUE);     // 蓝色
        for (size_t row = 400; row < 480; row++) // 黑白
        {
            for (size_t col = 0; col < 90; col++)
            {
                if (col >= 0 && col < 45)
                {
                    lcd_mmap[row][col] = BLACK;
                }
                else
                    lcd_mmap[row][col] = WHITE;
            }
        }
    }
    // 清屏图标
    show_bmp("./res/cscreen2.bmp", 0, 0, lcd_mmap);

    // 上边栏  撤回板块插入图片
    show_bmp("./res/w_and_f.bmp", 481, 0, lcd_mmap);
    // 150 90

    // 下载保存图片图标
    show_bmp("./res/save_pic.bmp", 641, 0, lcd_mmap);

    // 锁定图标
    show_bmp("./res/unlock.bmp", 100, 0, lcd_mmap);

    // 退出图标
    show_bmp("./res/exit.bmp", 736, 0, lcd_mmap);

    // 画册图标
    show_bmp("./res/picture.bmp", 736, 45, lcd_mmap);

    // 主体白色画板初始化
    for (size_t row = 100; row < 480; row++)
    {
        for (size_t col = 100; col < 800; col++)
        {
            lcd_mmap[row][col] = WHITE;
        }
    }

    // 上边栏颜色显示
    draw_ret_color(lcd_mmap);
}

// 左上角当前颜色显示板块
void draw_ret_color(int (*lcd_mmap)[800])
{
    for (size_t row = 0; row < 90; row++)
    {
        for (size_t col = 44; col < 89; col++)
        {
            lcd_mmap[row][col] = color[cr];
        }
    }
}

// 撤回功能存档函数
int (*withdraw_save(Lhead *head))[700]
{
    struct ElemNode *new = malloc(sizeof(struct ElemNode));
    if (!new)
    {
        return NULL; // 内存分配失败
    }
    new->last = malloc(sizeof(int[380][700]));
    if (!new->last)
    {
        free(new);
        return NULL; // 内存分配失败
    }
    list_add_tail(&new->list, &head->list);
    head->num++;
    head->num_tail = head->num;
    printf("\n%d\n", head->num);
    head->list_tail = &new->list;
    return new->last;
}

// 撤回功能释放函数
int (*withdraw_pull(Lhead *head))[700]
{
    if (head->num > 1)
    {
        head->list_tail = head->list_tail->prev;
        head->num--;
        int(*last)[700] = container_of(head->list_tail, struct ElemNode, list)->last;
        return last;
    }
    else
        return container_of(head->list.next, struct ElemNode, list)->last;
}

// 前进功能释放函数
int (*forward(Lhead *head))[700]
{
    if (head->num_tail > head->num)
    {
        head->list_tail = head->list_tail->next;
        head->num++;
        printf("%d\n", head->num);
        int(*last)[700] = container_of(head->list_tail, struct ElemNode, list)->last;
        return last;
    }
    else
        return container_of(head->list_tail, struct ElemNode, list)->last;
}

// 撤回链表初始化
Lhead *ListInt(int (*lcd_mmap)[800])
{
    struct Lhead *head = malloc(sizeof(struct Lhead));
    INIT_LIST_HEAD(&(head->list));
    struct ElemNode *new = malloc(sizeof(struct ElemNode));
    new->last = malloc(sizeof(int[380][700]));
    list_add_tail(&new->list, &head->list);
    head->num = 0;
    head->num++;
    printf("%d\n", head->num);
    head->num_tail = head->num;
    head->list_tail = &new->list;
    for (size_t row = 0; row < 380; row++) // 保存最初的白板作为头节点下的第一个节点
        for (size_t col = 0; col < 700; col++)
        {
            new->last[row][col] = WHITE;
        }
    return head;
}

// 登录界面按键响应
void pushnum(int x, int y, int x1, int x2, int y1, int y2, char num, char *buf, int *buf_lenth, int *log_ret)
{
    if (x >= x1 && x < x2 && y >= y1 && y < y2)
    {
        if (*buf_lenth == 9) // 设置账号字符上限
        {
            if (num == '-')
            {
                buf[--(*buf_lenth)] = '\0';
                printf("%s %d\n", buf, *buf_lenth);
            }
            else
                printf("enough!\n");
        }
        else
        {
            if ((num == '-') || (num == '+'))
            {
                if ((num == '-') && (*buf_lenth) != 0)
                {
                    buf[--(*buf_lenth)] = '\0';
                    printf("%s %d\n", buf, *buf_lenth);
                }
                else if ((num == '+') && (*buf_lenth) != 0)
                {
                    if (*log_ret == 1)
                        if (login_user(buf_ID, buf_PS))
                            logdraw = 1;
                    *log_ret = 1;
                }
                else if ((num == '+') && (*buf_lenth) == 0)
                {

                    font_login_more(220, 280, "请输入用户名和密码", 30);
                    sleep(1);
                    font_login_more(220, 280, "", 30);
                }
            }
            else
            {
                printf("%c\n", num);
                printf("%s %d\n", buf, *buf_lenth);
                buf[(*buf_lenth)++] = num;
            }
        }
    }
}

// 登录界面响应
int loginmenu(int x, int y, char *buf, int *buf_lenth, int *log_ret)
{

    pushnum(x, y, 494, 594, 115, 186, '1', buf, buf_lenth, log_ret); // 1
    pushnum(x, y, 594, 694, 115, 186, '2', buf, buf_lenth, log_ret); // 2
    pushnum(x, y, 694, 794, 115, 186, '3', buf, buf_lenth, log_ret); // 3

    pushnum(x, y, 494, 594, 186, 257, '4', buf, buf_lenth, log_ret); // 4
    pushnum(x, y, 594, 694, 186, 257, '5', buf, buf_lenth, log_ret); // 5
    pushnum(x, y, 694, 794, 186, 257, '6', buf, buf_lenth, log_ret); // 6

    pushnum(x, y, 494, 594, 257, 328, '7', buf, buf_lenth, log_ret); // 7
    pushnum(x, y, 594, 694, 257, 328, '8', buf, buf_lenth, log_ret); // 8
    pushnum(x, y, 694, 794, 257, 328, '9', buf, buf_lenth, log_ret); // 9

    pushnum(x, y, 494, 594, 328, 399, '-', buf, buf_lenth, log_ret); // 回退
    pushnum(x, y, 594, 694, 328, 399, '0', buf, buf_lenth, log_ret); // 0
    pushnum(x, y, 694, 794, 328, 399, '+', buf, buf_lenth, log_ret); // 确定
}

// 登录主函数
void login(int x, int y, char *buf, int *buf_lenth, int *log_ret)
{
    if ((*log_ret) == 0)
    {
        loginmenu(x, y, buf, buf_lenth, log_ret); // 输入
        font_login(240, 180, buf);                // 显示
    }
    else
    {
        loginmenu(x, y, buf, buf_lenth, log_ret);
        font_login(240, 230, buf);
    }
}

// 绘制一个点并考虑线条的半径,优化断点
void drawCircle(int (*lcd_mmap)[800], int x, int y, int color, int r)
{
    for (int i = -r; i <= r; i++)
    {
        for (int j = -r; j <= r; j++)
        {
            int new_x = x + i;
            int new_y = y + j;
            // 边界检查，确保坐标在屏幕范围内
            if (new_x >= 100 && new_x < 800 && new_y >= 100 && new_y < 480)
            {
                // 判断是否在圆形的范围内
                if (i * i + j * j < r * r)
                {
                    lcd_mmap[new_y][new_x] = color;
                }
            }
        }
    }
}

int main()
{
    int lcd_fd = open("/dev/fb0", O_RDWR);
    int(*lcd_mmap)[800] = (int(*)[800])mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);

    int x = -1, y = -1; // 初始值是-1
    int x0 = -1, y0 = -1;
    int flag_line_start = 0;

    int touch_fd = open("/dev/input/event0", O_RDWR);
    if (touch_fd < 0)
    {
        printf("open touch failed\n");
        return -1;
    }

    struct input_event touch_event;

    int temp1 = 0, temp2 = -1; // 判断xy是否都有变化
    int r_ret = 0;             // 判断是否刷新半径球
    // int radius = 10;

    int login_ret = 0;                      // 判断是填写用户名还是密码
    struct Lhead *head = ListInt(lcd_mmap); // 撤回链表初始化

    // show_bmp("./res/login.bmp", 0, 0, lcd_mmap);

    while (1)
    {
        show_bmp("./res/login.bmp", 0, 0, lcd_mmap);
        for (int i = 0; i < 10; i++)
        {
            buf_ID[i] = 0; // 登录界面按键存档
            buf_PS[i] = 0; // 登录界面按键存档
            buf_PS_len = 0;
        }
        buf_PS_len = 0;
        buf_ID_len = 0;
        login_ret = 0;

        // 登录界面循环
        while (logdraw != 1)
        {
            int read_ret = read(touch_fd, &touch_event, sizeof(struct input_event));
            if (read_ret < 0)
            {
                perror("read touch failed\n");
                return -1;
            }

            if (touch_event.type == EV_ABS)
            {
                if (touch_event.code == ABS_X)
                {
                    x = touch_event.value; // 蓝色板子
                    // x = touch_event.value * 800 / 1024; // 黑色板子
                }
                else if (touch_event.code == ABS_Y)
                {
                    y = touch_event.value;
                    // y = touch_event.value * 480 / 600;
                }
            }
            printf("x=%d,y=%d\n", x, y);

            if (x > 137 && x < 247 && y > 323 && y < 363) // 注册
            {
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                {
                    if (buf_ID[0] == '\0' || buf_PS[0] == '\0')
                    {
                        font_login_more(220, 280, "请输入用户名和密码", 30);
                        sleep(1);
                        font_login_more(220, 280, "", 30);
                    }
                    else
                        register_user(buf_ID, buf_PS);
                }
            }

            if (x > 255 && x < 366 && y > 323 && y < 363) // 注册
            {
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                {
                    if (buf_ID[0] == '\0' || buf_PS[0] == '\0')
                    {
                        font_login_more(220, 280, "请输入用户名和密码", 30);
                        sleep(1);
                        font_login_more(220, 280, "", 30);
                    }
                    else
                    {
                        if (login_user(buf_ID, buf_PS))
                            logdraw = 1;
                    }
                }
            }

            // 登录界面
            if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
            {
                if (x > 240 && x < 450 && y > 180 && y < 220) // 触摸哪里输入哪里
                    login_ret = 0;
                else if (x > 240 && x < 450 && y > 230 && y < 270)
                    login_ret = 1;
                if (login_ret == 0)
                    login(x, y, buf_ID, &buf_ID_len, &login_ret); // 登录账号
                else
                    login(x, y, buf_PS, &buf_PS_len, &login_ret);
            }
            if (login_ret == 0)
            {
                show_bmp("./res/write.bmp", 455, 230, lcd_mmap);
                show_bmp("./res/IO.bmp", 455, 180, lcd_mmap);
            }
            else
            {
                show_bmp("./res/write.bmp", 455, 180, lcd_mmap);
                show_bmp("./res/IO.bmp", 455, 230, lcd_mmap);
            }
        }

        // 初始化画板循环的条件
        if (logdraw == 1)
        {
            draw_face(lcd_mmap); // 画面初始化
            x = -1;
            y = -1;
        }

        // 画板循环
        while (logdraw == 1)
        {
            int read_ret = read(touch_fd, &touch_event, sizeof(struct input_event));
            if (read_ret < 0)
            {
                perror("read touch failed\n");
                return -1;
            }

            if (touch_event.type == EV_ABS)
            {
                if (touch_event.code == ABS_X)
                {
                    x = touch_event.value; // 蓝色板子
                    // x = touch_event.value * 800 / 1024; // 黑色板子
                }
                else if (touch_event.code == ABS_Y)
                {
                    y = touch_event.value;
                    // y = touch_event.value * 480 / 600;
                }
            }
            printf("x=%d,y=%d\n", x, y);

            draw_ret_color(lcd_mmap); // 左上角显示当前颜色

            // 撤回功能
            if ((x >= 481 && x < 556) && (y >= 0 && y < 90))
            {
                if (touch_my == 0)
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                        touch_my = 1;
                }
                else if (touch_my == 1)
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        int(*last)[700] = withdraw_pull(head);
                        for (size_t row = 0; row < 380; row++)
                            for (size_t col = 0; col < 700; col++)
                            {
                                lcd_mmap[row + 100][col + 100] = last[row][col];
                            }
                        touch_my = 0;
                        withfor=1;
                    }
                
            }

            // 前进功能
            if ((x >= 556 && x < 631) && (y >= 0 && y < 90) && withfor!=2)
            {
                if (touch_my == 0)
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                        touch_my = 1;
                }
                else if (touch_my == 1)
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        int(*last)[700] = forward(head);
                        for (size_t row = 0; row < 380; row++)
                            for (size_t col = 0; col < 700; col++)
                            {
                                lcd_mmap[row + 100][col + 100] = last[row][col];
                            }
                        touch_my = 0;
                    }
            }

            // 画出小球
            if (y > 100 && y < 480 && x > 100 && x < 800)
            {
                // 检测按下触摸屏，记录起点
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                {
                    x0 = x;
                    y0 = y;
                    flag_line_start = 1; // 设置标志，表示下一次触摸将是终点
                }
                // 绘制线条的逻辑
                if (flag_line_start && touch_event.type == EV_SYN)
                {
                    if (x0 != -1 && y0 != -1) // 确保起点有效
                    {
                        // 计算点之间的距离
                        int dx = abs(x - x0);
                        int dy = abs(y - y0);
                        // 选择步数为较大的轴差值
                        int steps = dx > dy ? dx : dy;
                        float x_inc = (float)(x - x0) / (float)steps;
                        float y_inc = (float)(y - y0) / (float)steps;
                        float x_temp = x0;
                        float y_temp = y0;
                        // 使用插值法绘制点，确保线条连续
                        for (int i = 0; i <= steps; i++)
                        {
                            // 绘制当前点
                            drawCircle(lcd_mmap, (int)x_temp, (int)y_temp, color[cr], rd);
                            x_temp += x_inc;
                            y_temp += y_inc;
                        }
                        x0 = x;
                        y0 = y;
                    }
                }
                // 检测松开触摸屏，重置起点和标志
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                {
                    flag_line_start = 0;
                    x0 = -1;
                    y0 = -1;
                }
            }

            // 撤回存储
            if ((x >= 100 && x < 800) && (y >= 100 && y < 480))
            {
                if (touch_my == 0)
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                        touch_my = 1;
                }
                else if (touch_my == 1)
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        if(withfor==1)
                            withfor=2;
                        int(*last)[700] = withdraw_save(head);
                        for (size_t row = 0; row < 380; row++)
                        {
                            for (size_t col = 0; col < 700; col++)
                            {
                                last[row][col] = lcd_mmap[row + 100][col + 100];
                            }
                        }
                        touch_my = 0;
                    }
            }

            // 左边栏触摸反应
            if (stop != 1)
            {
                // 红色
                if ((x >= 0 && x < 90) && (y >= 100 && y < 200))
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        vcol = 2;
                        cr = 0;
                    }
                }
                // 绿色
                if ((x >= 0 && x < 90) && (y >= 200 && y < 300))
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        vcol = 3;
                        cr = 1;
                    }
                }
                // 蓝色
                if ((x >= 0 && x < 90) && (y >= 300 && y < 400))
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        vcol = 4;
                        cr = 2;
                    }
                }
                // 变色版本
                if ((x >= 0 && x < 90) && (y >= 400 && y < 480))
                {
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        vcol = 1; // 进入变色模式
                        if (x >= 0 && x < 45)
                            cr = 4;
                        else
                            cr = 5;
                    }
                }
            }

            // 上边栏显示

            // 滑动调整块----显示

            // 滑动区三角背景色
            {
                int vary_size = 1; // 为设置三角形的辅助变量，每三列则往上加一行长度
                if (cr == 5)
                    for (size_t col = 100; col < 369; col++)
                    {
                        if (col % 3 == 0)
                            vary_size += 1;
                        for (size_t row = 0; row < 90 - vary_size; row++)
                        {
                            if (!(row > 0 && row < 45 && col > 100 && col < 145))
                                lcd_mmap[row][col] = BLACK;
                        }
                    }
                else
                    for (size_t col = 100; col < 369; col++)
                    {
                        if (col % 3 == 0)
                            vary_size += 1;
                        for (size_t row = 0; row < 90 - vary_size; row++)
                        {
                            if (!(row > 0 && row < 45 && col > 100 && col < 145))
                                lcd_mmap[row][col] = WHITE;
                        }
                    }

                // 滑动主色系
                vary_size = 1;
                for (size_t col = 100; col <= 370; col++)
                {
                    if (col % 3 == 0)
                        vary_size += 1;
                    for (size_t row = 90 - vary_size; row < 90; row++)
                    {
                        lcd_mmap[row][col] = color[cr];
                        int x = col;
                        if (vcol == 1) // 滑动多色系
                        {
                            if (x >= 100 && x <= 151)
                                lcd_mmap[row][col] = 0x00ff0000 + (x - 100) * 5 * 256;
                            else if (x > 151 && x <= 202)
                                lcd_mmap[row][col] = 0x00ffff00 - (x - 151) * 5 * 65536;
                            else if (x > 202 && x <= 253)
                                lcd_mmap[row][col] = 0x0000ff00 + (x - 202) * 5 * 1;
                            else if (x > 253 && x <= 304)
                                lcd_mmap[row][col] = 0x0000ffff - (x - 253) * 5 * 256;
                            else if (x > 304 && x <= 355)
                                lcd_mmap[row][col] = 0x000000ff + (x - 304) * 5 * 65536;
                            else if (x > 355 && x <= 370)
                                lcd_mmap[row][col] = 0x00ff00ff - (x - 335) * 5 * 1;
                        }
                        else if (vcol == 2) // 红色系
                        {
                            if (col <= 235)
                                lcd_mmap[row][col] = 0x00ff8787 - (col - 100) * (1 + 256);
                            else
                                lcd_mmap[row][col] = 0x00ff0000 - (col - 235) * 65536;
                        }
                        else if (vcol == 3) // 绿色系
                        {
                            if (col <= 235)
                                lcd_mmap[row][col] = 0x0087ff87 - (col - 100) * (1 + 65536);
                            else
                                lcd_mmap[row][col] = 0x0000ff00 - (col - 235) * 256;
                        }
                        else if (vcol == 4) // 蓝色系
                        {
                            if (col <= 235)
                                lcd_mmap[row][col] = 0x008787ff - (col - 100) * (256 + 65536);
                            else
                                lcd_mmap[row][col] = 0x000000ff - (col - 235) * 1;
                        }
                    }
                }
            }

            // 解锁滑动区
            {
                if (stop == 1)
                {

                    if (y > 0 && y < 45 && x > 100 && x < 200)
                    {
                        if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                        {
                            show_bmp("./res/unlock.bmp", 100, 0, lcd_mmap);
                            printf("unlock\n");
                            stop = 0;
                        }
                    }
                }
                else
                {
                    if (y > 0 && y < 45 && x > 100 && x < 200) // 锁住滑动区
                    {
                        if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                        {
                            show_bmp("./res/lock.bmp", 100, 0, lcd_mmap);
                            printf("lock\n");
                            stop = 1;
                        }
                    }
                }
            }

            // 调整画笔大小+画笔颜色-----触摸反应
            if (stop != 1)
            {
                if (touch_event.type == EV_SYN)
                    if ((x > 100 && x <= 370) && (y > 0 && y < 90) && !(y > 0 && y < 45 && x > 100 && x < 200))
                    {
                        if (vcol == 0) // 调整半径
                            rd = (x - 100) / 6;
                        else if (vcol == 1) // 变色系
                        {
                            if (x > 100 && x <= 151)
                                color[6] = 0x00ff0000 + (x - 100) * 5 * 256;
                            else if (x > 151 && x <= 202)
                                color[6] = 0x00ffff00 - (x - 151) * 5 * 65536;
                            else if (x > 202 && x <= 253)
                                color[6] = 0x0000ff00 + (x - 202) * 5 * 1;
                            else if (x > 253 && x <= 304)
                                color[6] = 0x0000ffff - (x - 253) * 5 * 256;
                            else if (x > 304 && x <= 355)
                                color[6] = 0x000000ff + (x - 304) * 5 * 65536;
                            else if (x > 355 && x <= 370)
                                color[6] = 0x00ff00ff - (x - 335) * 5 * 1;
                            cr = 6;
                        }
                        else if (vcol == 2) // 红色
                        {
                            if (x <= 235)
                                color[6] = 0x00ff8787 - (x - 100) * (1 + 256);
                            else
                                color[6] = 0x00ff0000 - (x - 235) * 65536;
                            cr = 6;
                        }
                        else if (vcol == 3) // 绿色
                        {
                            if (x <= 235)
                                color[6] = 0x0087ff87 - (x - 100) * (1 + 65536);
                            else
                                color[6] = 0x0000ff00 - (x - 235) * 256;
                            cr = 6;
                        }
                        else if (vcol == 4) // 蓝色
                        {
                            if (x <= 235)
                                color[6] = 0x008787ff - (x - 100) * (256 + 65536);
                            else
                                color[6] = 0x000000ff - (x - 235) * 1;
                            cr = 6;
                        }
                    }
            }

            // 上边栏半径显示球
            {
                for (size_t row = 0; row < 90; row++) // 半径小球
                {
                    for (size_t col = 380; col <= 470; col++)
                    {
                        if ((row - 45) * (row - 45) + (col - 425) * (col - 425) <= rd * rd)
                        {
                            lcd_mmap[row][col] = color[cr];
                        }
                        else
                        {
                            if (cr != 5)
                                lcd_mmap[row][col] = WHITE;
                            else
                                lcd_mmap[row][col] = BLACK;
                        }
                    }
                }
            }

            // 点圆用以调整半径
            if (stop != 1)
            {
                if (x >= 381 && x <= 470 && y > 0 && y < 90)
                    if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                    {
                        vcol = 0;
                    }
            }

            // 保存功能
            if (x >= 641 && x <= 725 && y >= 0 && y < 90)
            {
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                {
                    appendArrayToFile(lcd_mmap, buf_ID);
                    font_sa_rd(350, 220, "保存成功！");
                    sleep(1);
                    read_tail_ArrayFromFile(lcd_mmap, buf_ID);
                }
            }

            // 读取功能
            if (x >= 736 && x < 800 && y >= 45 && y < 90)
            {
                // if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                // {
                readArrayFromFile(lcd_mmap, buf_ID);
                show_bmp("./res/exit1.bmp", 736, 0, lcd_mmap);
                int dir_out = 0;
                int time_go = 0;
                while (dir_out == 0)
                {
                    int dir = get_direction();
                    switch (dir)
                    {
                    case RIGHT:
                        time_go--;
                        read_pre_ArrayFromFile(lcd_mmap, buf_ID, &time_go);
                        show_bmp("./res//exit1.bmp", 736, 0, lcd_mmap);
                        printf("left\n");
                        break;
                    case LEFT:
                        time_go++;
                        read_next_ArrayFromFile(lcd_mmap, buf_ID, &time_go);
                        show_bmp("./res//exit1.bmp", 736, 0, lcd_mmap);
                        break;
                    case UP:
                        printf("up\n");
                        x = -1;
                        y = -1;
                        dir_out = 1;
                        close(touch_fd);
                        touch_fd = -1;
                        break;
                    case DOWN:
                        printf("down\n");
                        break;
                    case -1:
                        printf("未识别\n");
                        break;
                    }
                }
                // }
            }
            if (touch_fd < 0)
            {
                touch_fd = open("/dev/input/event0", O_RDWR);
                // 退出图标
                show_bmp("./res/exit.bmp", 736, 0, lcd_mmap);

                // 画册图标
                show_bmp("./res/picture.bmp", 736, 45, lcd_mmap);
            }

            // 返回功能
            if (x >= 736 && x < 800 && y >= 0 && y < 45)
            {
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
                {
                    logdraw = 0;
                    stop = 0;
                }
            }

            // 清屏
            if ((x > 0 && x < 45) && (y > 0 && y < 100))
            {
                if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
                {
                    int(*last)[700] = withdraw_save(head);
                    for (size_t row = 100; row < 480; row++)
                    {
                        for (size_t col = 100; col < 800; col++)
                        {
                            lcd_mmap[row][col] = WHITE;
                            last[row - 100][col - 100] = WHITE;
                        }
                    }
                }
            }
        }
    }
    close(touch_fd);
    return 0;
}
