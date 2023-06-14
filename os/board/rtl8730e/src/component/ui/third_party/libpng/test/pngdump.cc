#include "png.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "zlib.h"
#define PNG_BYTES_TO_CHECK 4
#define _POSIX_SOURCE 1

typedef struct {                     /**** BMP file header structure ****/
	unsigned int   bfSize;           /* Size of file */
	unsigned short bfReserved1;      /* Reserved */
	unsigned short bfReserved2;      /* ... */
	unsigned int   bfOffBits;        /* Offset to bitmap data */
} MyBITMAPFILEHEADER;
typedef struct {                     /**** BMP file info structure ****/
	unsigned int   biSize;           /* Size of info header */
	int            biWidth;          /* Width of image */
	int            biHeight;         /* Height of image */
	unsigned short biPlanes;         /* Number of color planes */
	unsigned short biBitCount;       /* Number of bits per pixel */
	unsigned int   biCompression;    /* Type of compression to use */
	unsigned int   biSizeImage;      /* Size of image data */
	int            biXPelsPerMeter;  /* X pixels per meter */
	int            biYPelsPerMeter;  /* Y pixels per meter */
	unsigned int   biClrUsed;        /* Number of colors used */
	unsigned int   biClrImportant;   /* Number of important colors */
} MyBITMAPINFOHEADER;
void  saveToImg(const char *filename, unsigned char *rgbbuf, int width, int height)
{
	MyBITMAPFILEHEADER bfh;
	MyBITMAPINFOHEADER bih;
	/* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
	unsigned short bfType = 0x4d42;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfSize = 2 + sizeof(MyBITMAPFILEHEADER) + sizeof(MyBITMAPINFOHEADER) + (width + 4 - width % 4) * height * 3;
	bfh.bfOffBits = 0x36;

	bih.biSize = sizeof(MyBITMAPINFOHEADER);
	bih.biWidth = width;
	bih.biHeight = -height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;
	bih.biSizeImage = (width + 4 - width % 4) * height * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	int BytesPerLine = 3 * width;
	while (BytesPerLine % 4 != 0) {
		BytesPerLine ++;
	}

	FILE *file = fopen(filename, "wb");
	if (!file) {
		printf("Could not write file\n");
		return;
	}

	/*Write headers*/
	fwrite(&bfType, sizeof(bfType), 1, file);
	fwrite(&bfh, sizeof(bfh), 1, file);
	fwrite(&bih, sizeof(bih), 1, file);

	//fwrite(rgbbuf,width*height*3,1,file);

	unsigned char b = 0;//用于填充
	for (int i = 0 ; i < width * height ; i ++) {
		//32位位图图像的格式为：Blue, Green, Red, Alpha
		fwrite(&(rgbbuf[i * 4]), 1, 1, file);
		fwrite(&(rgbbuf[i * 4 + 1]), 1, 1, file);
		fwrite(&(rgbbuf[i * 4 + 2]), 1, 1, file);
		if (i % width == width - 1) { //填充字节
			for (int k = 0 ; k < (BytesPerLine - width * 3) ; k ++) {
				fwrite(&b, sizeof(unsigned char), 1, file);
			}
		}
	}
	fclose(file);
}

void read_png(char *file_name)  /* We need to open the file */
{
	png_structp png_ptr;
	png_infop info_ptr;
	int i;
	png_bytep *row_pointers;
	int sig_read = 0;
	char buf[PNG_BYTES_TO_CHECK];
	png_uint_32 width, height;
	int bit_depth,  interlace_type;
	FILE *fp;
	int w, h, x, y, temp, color_type;
	unsigned char buffer[1 * 1000 * 1000] = {0};
	int count = 0;
	int png_transforms;
	if ((fp = fopen(file_name, "rb")) == NULL) {
		return ;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	/* 为这个结构体分配空间 存储这图像的信息*/
	info_ptr = png_create_info_struct(png_ptr);
	setjmp(png_jmpbuf(png_ptr));
	/* 若读到的数据并没有PNG_BYTES_TO_CHECK个字节 */
	temp = fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
	if (temp < PNG_BYTES_TO_CHECK) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		printf("没有充足的字节\n");
		return /* 返回值 */;
	}
	temp = png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
	if (temp != 0) {
		fclose(fp);
		printf("不匹配字符串\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return /* 返回值 */;
	}
	/* 复位文件指针 */
	rewind(fp);
	/*如果我们已经读取了这些信息  跳过相应的函数*/
	/* 开始读文件 */
	png_init_io(png_ptr, fp);
	/* 获取图像的色彩类型 */
	png_read_png(png_ptr, info_ptr, png_transforms, NULL);
	color_type = png_get_color_type(png_ptr, info_ptr);
	/* 获取图像的宽高 */
	//color_type = info_ptr->color_type;
	printf("color_type=%d\n", color_type);
	w = png_get_image_width(png_ptr, info_ptr);
	h = png_get_image_height(png_ptr, info_ptr);
	/* 获取图像的所有行像素数据，row_pointers里边就是rgba数据 */
	row_pointers = png_get_rows(png_ptr, info_ptr);
	/* 根据不同的色彩类型进行相应处理 */
	switch (color_type) {
	case PNG_COLOR_TYPE_RGB_ALPHA:
		for (y = 0; y < h; ++y) {
			for (x = 0; x < w * 4;) {
				/* 以下是RGBA数据，需要自己补充代码，保存RGBA数据 */
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // red
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // green
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // blue
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // alpha
			}
		}
		break;

	case PNG_COLOR_TYPE_RGB:
		for (y = 0; y < h; ++y) {
			for (x = 0; x < w * 3;) {
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // red
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // green
				/* 目标内存 */buffer[count++] = row_pointers[y][x++]; // blue
			}
		}
		break;
	/* 其它色彩类型的图像就不读了 */
	default:
		fclose(fp);
		temp = PNG_COLOR_TYPE_RGB_ALPHA;
		printf("其他格式不读了%d \n", color_type);
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		return /* 返回值 */;
	}
	/* One of the following I/O initialization methods is REQUIRED */
	printf("count=%d\n", count);
#if  1
	saveToImg("./a.bmp", buffer, w, h);
//#else
	for (i = 0; i < 200  ; i += 4) {
		printf("%3d%3d%3d%3d -- ", buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
	}
#endif
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	printf("\n");
	return ;
}

int main(int argc, char **argv)
{
	FILE *fp;
	int temp = 0;
	char buf[PNG_BYTES_TO_CHECK];
	/* 打开源头文件 */
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		return 0;
	}

	/* 读取标志位 如果不是png文件退出 */
	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK) {
		printf("Sorry,it is not the png file");
		return 0;
	} else {
		// 非0 表示匹配
		temp = (!png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
		printf("yes ,goog luck %d \n ", temp);
		fclose(fp);
	}
	read_png(argv[1]);
	//printf("%d",check_if_png("hei.png",fp));
	return 0;
}