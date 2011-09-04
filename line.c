/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 19:40:52 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "pixel.h"
//#include "line.h"

#define DEBUG 1

int main(int argc, char *argv[])
{
	struct framebuffer fb;
	unsigned char *fb_startp;
	int x, y;

	//step 1. get frame buffer point
	//unsigned char *fb_open(char *fbname, struct framebuffer *fbp)
	fb_startp = fb_open("/dev/fb0", &fb);
	//step 2. draw pixel
	for(y = 0; y < fb.fb_vinfo.yres; y++){
		for(x = 0; x < fb.fb_vinfo.xres; x++){
			fb_draw_pixel(&fb, x, y, 0xff00ff);
		}
	}
	//step 3. munmap

	return 0;
}

