#include "font.h"

// 初始化Lcd
struct LcdDevice *init_lcd(const char *device)
{
	// 申请空间
	struct LcdDevice *lcd = malloc(sizeof(struct LcdDevice));
	if (lcd == NULL)
	{
		return NULL;
	}

	// 1打开设备
	lcd->fd = open(device, O_RDWR);
	if (lcd->fd < 0)
	{
		perror("open lcd fail");
		free(lcd);
		return NULL;
	}

	// 映射
	lcd->mp = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd->fd, 0);

	return lcd;
}

// 显示字体
void font_login(int x, int y, char *buf)
{

	struct LcdDevice *lcd = init_lcd("/dev/fb0");
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	// 字体大小的设置
	fontSetSize(f, 50);
	bitmap *bm = createBitmapWithInit(210, 40, 4, getColor(255, 245, 245, 245)); // 也可使用createBitmap函数，改变画板颜色
	// 将字体写到点阵图上
	fontPrint(f, bm, 0, 0, buf, getColor(0, 0, 0, 0), 200);
	// 把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->mp, x, y, bm);
	// 关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
}

//显示字体可调大小
void font_login_more(int x, int y, char *buf,int size)
{

	struct LcdDevice *lcd = init_lcd("/dev/fb0");
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	// 字体大小的设置
	fontSetSize(f, size);
	bitmap *bm = createBitmapWithInit(210, 40, 4, getColor(255, 245, 245, 245)); // 也可使用createBitmap函数，改变画板颜色
	// 将字体写到点阵图上
	fontPrint(f, bm, 0, 0, buf, getColor(0, 0, 0, 255), 200);
	// 把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->mp, x, y, bm);
	// 关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
}


//保存，读取专用
void font_sa_rd(int x, int y, char *buf)
{

	struct LcdDevice *lcd = init_lcd("/dev/fb0");
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	// 字体大小的设置
	fontSetSize(f, 50);
	bitmap *bm = createBitmapWithInit(200, 40, 4, getColor(255, 255, 255, 255)); // 也可使用createBitmap函数，改变画板颜色
	// 将字体写到点阵图上
	fontPrint(f, bm, 0, 0, buf, getColor(0, 0, 0, 0), 200);
	// 把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->mp, x, y, bm);
	// 关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
}