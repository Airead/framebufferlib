/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 22:48:10 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#ifndef FB_LINE
#define FB_LINE

#include "framebuffer.h"
#include "pixel.h"

int fb_draw_line(FB *fbp, FB_POINT *point1, FB_POINT *point2);
int fb_draw_line_screen(FB_SCREEN *screenp, FB_POINT *point1, FB_POINT *point2);
int fb_draw_line_screen_trans(FB_SCREEN *screenp, FB_POINT *point1, FB_POINT *point2,
			      unsigned char trans);

#endif
