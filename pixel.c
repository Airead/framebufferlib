/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 20:53:11 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#include "pixel.h"


/*
 * Draw a pixel (x, y) at framebuffer with color
 */
int fb_draw_pixel(struct framebuffer *fbp, int x, int y, COLOR_32 color)
{
	unsigned char *p;	/* point framebuffer (x, y) pixel */
	unsigned long locate;

	locate = (y * fbp->fb_vinfo.xres + x) 
		* fbp->fb_vinfo.bits_per_pixel / 8;
	
	p = fbp->fb_starting + locate;

	*p++ = (color >> 0) & 0xff; /* blue */
	*p++ = (color >> 8) & 0xff; /* green */
	*p++ = (color >> 16) & 0xff; /* red */
	*p++ = (color >> 24) & 0xff; /* transp */

	return 1;
}

