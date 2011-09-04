/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 21:03:11 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#ifndef FB_PIXEL
#define FB_PIXEL

#include "framebuffer.h"

typedef unsigned long COLOR_32;	/* 0x0f0f0f0f: transp, red, green, blue*/

int fb_draw_pixel(struct framebuffer *fbp, int x, int y, COLOR_32 color);

#endif
