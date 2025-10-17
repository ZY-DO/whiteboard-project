#include "determine_dire.h"
int get_direction()
{
    int sx = -1, sy = -1;
    int cx = -1, cy = -1; // 初始值是-1
    int touch_fd = open("/dev/input/event0", O_RDWR);
    if (touch_fd < 0)
    {
        printf("open touch failed\n");
        return -1;
    }

    struct input_event touch_event;
    while (1)
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
                cx = touch_event.value;
            }
            else if (touch_event.code == ABS_Y)
            {
                cy = touch_event.value;
            }
        }

        // 按下
        if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 1)
        {
            sx = cx;
            sy = cy;
            if (sx >= 736 && sx < 800 && sy >= 0 && sy < 90)
            {
                 close(touch_fd);
                return UP;
            }
        }

        // 松手
        if (touch_event.type == EV_KEY && touch_event.code == BTN_TOUCH && touch_event.value == 0)
        {

            if (sx != cx || sy != cy) 
            {
                int dir_h = -1;
                int dir_v = -1;
                if (abs(cx - sx) > 100)
                {
                    if (cx > sx)
                    {
                        dir_h = RIGHT;
                    }
                    else if (cx < sx)
                    {
                        dir_h = LEFT;
                    }
                }

                if (abs(cy - sy) > 100)
                {
                    if (cy > sy)
                    {
                        dir_v = DOWN;
                    }
                    else if (cy < sy)
                    {
                        dir_v = UP;
                    }
                }

                if (abs(cx - sx) > abs(cy - sy))
                {
                    return dir_h;
                }
                else if (abs(cx - sx) < abs(cy - sy))
                {
                    return dir_v;
                }
            }

            return -1;
        }
    }

    close(touch_fd);
}
