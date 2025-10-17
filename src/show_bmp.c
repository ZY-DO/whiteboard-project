#include "show_bmp.h"
int show_bmp(const char *bmp_file_name, int x, int y, int (*lcd_mmap)[800])
{
    FILE *fp = fopen(bmp_file_name, "rb");
    if (fp == NULL)
    {
        perror("open bmp file error");
        return -1;
    }

    struct BitMapFileHeader bmp_file_header;
    struct BitMapInfoHeader bmp_info_header;

    fread(&bmp_file_header, sizeof(bmp_file_header), 1, fp);
    fread(&bmp_info_header, sizeof(bmp_info_header), 1, fp);


    int width = bmp_info_header.biWidth;
    int height = bmp_info_header.biHeight;

    // Memory allocation for image buffer
    unsigned char(*bmp_buf)[width * 4] = malloc(height * width * 4);
    if (bmp_buf == NULL)
    {
        perror("malloc error");
        fclose(fp);
        return -1;
    }

    if (bmp_file_header.bfType == 0x4D42 && bmp_info_header.biBitCount == 32)
    {
        int padding = (4 - (width * 4) % 4) % 4;

        for (int row = 0; row < height; row++)
        {
            fread(bmp_buf[row], width * 4, 1, fp);
            fseek(fp, padding, SEEK_CUR);
        }

        if (y + height > 480 || x + width > 800)
        {
            fprintf(stderr, "bmp size is too big\n");
            free(bmp_buf);
            fclose(fp);
            return -1;
        }

        for (int row = 0; row < height; row++)
        {
            for (int col = 0; col < width; col++)
            {
                unsigned char blue = bmp_buf[height - 1 - row][col * 4];
                unsigned char green = bmp_buf[height - 1 - row][col * 4 + 1];
                unsigned char red = bmp_buf[height - 1 - row][col * 4 + 2];
                // Alpha channel is bmp_buf[height - 1 - row][col * 4 + 3], but we ignore it for now
                lcd_mmap[row + y][col + x] = (red << 16) | (green << 8) | blue;
            }
        }

        free(bmp_buf);
    }
    else if (bmp_file_header.bfType == 19778 && bmp_info_header.biBitCount == 24)
    {
        int padding = 4 - width * 3 % 4;
        for (size_t row = 0; row < height; row++)
        {
            int ret = fread(bmp_buf[row], width * 3, 1, fp);
            fseek(fp, padding, SEEK_CUR);
        }
        if (y + height > 480 || x + width > 800)
        {
            printf("bmp size is too big\n");
            return -1;
        }
        for (size_t row = 0; row < height; row++)
        {
            for (size_t col = 0; col < width; col++)
            {
                lcd_mmap[row + y][col + x] = 0x00 << 24 | bmp_buf[height - 1 - row][col * 3 + 2] << 16 | bmp_buf[height - 1 - row][col * 3 + 1] << 8 | bmp_buf[height - 1 - row][col * 3] << 0;
            }
        }
        free(bmp_buf);
    }
    else
    {
        fprintf(stderr, "not 32-bit or 24-bit bmp file: %s\n", bmp_file_name);
    }

    fclose(fp);
    return 0;
}
