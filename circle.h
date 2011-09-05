/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 05 10:39:32 CST			*
 ********************************************************
 *		after studying C 49 days		*
 *		after studying APUE 14 days		*
 ********************************************************/

#ifndef FB_CIRCLE
#define FB_CIRCLE

#include "framebuffer.h"
#include "pixel.h"

int fb_circle_moveh(int *x, int *y, int *d);
int fb_circle_moved(int *x, int *y, int *d);
int fb_circle_movev(int *x, int *y, int *d);
int fb_draw_circle(FB *fbp, FB_POINT *center, float radius);

#endif
