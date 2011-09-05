/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 05 10:17:50 CST			*
 ********************************************************
 *		after studying C 49 days		*
 *		after studying APUE 14 days		*
 ********************************************************/

#include "circle.h"

/*
 * move horizontally
 */
int fb_circle_moveh(int *x, int *y, int *d)
{
	*x = *x + 1;
	*d = *d + *x * 2 + 1;

	return 0;
}

int fb_circle_moved(int *x, int *y, int *d)
{
	*x = *x + 1;
	*y = *y - 1;
	*d = *d + *x * 2 - *y * 2 + 2;

	return 0;
}

int fb_circle_movev(int *x, int *y, int *d)
{
	*y = *y - 1;
	*d = *d - *y * 2 + 1;

	return 0;
}

/*
 * Draw a circle
 */
int fb_draw_circle(FB *fbp, FB_POINT *center, float radius)
{
	/* Bresenham's incremental circle algorithm for the first quadrant
	 * all variables are assumed integer */
	FB_POINT point;
	int x, y;
	int d, limit, e;

	/* initialize the variables */
	x = 0;
	y = radius + 0.5;
	d = (1 - radius) * 2;
	limit = 0;

	while( y >= limit){
		fb_set_pixel(&point, x + center->x, y + center->y, center->color);
		fb_draw_pixel(fbp, &point);
		fb_set_pixel(&point, -x + center->x, y + center->y, center->color);
		fb_draw_pixel(fbp, &point);
		fb_set_pixel(&point, x + center->x, -y + center->y, center->color);
		fb_draw_pixel(fbp, &point);
		fb_set_pixel(&point, -x + center->x, -y + center->y, center->color);
		fb_draw_pixel(fbp, &point);
		/* determine if case 1 or 2, 4 or 5, or 3 */
		if(d < 0){
			e = 2 * d + 2 * y - 1;
			/* determine whether case 1 or 2 */
			if(e <= 0){
				fb_circle_moveh(&x, &y, &d);
			}else{
				fb_circle_moved(&x, &y, &d);
			}
		}else if(d > 0){
			e = 2 * d - 2 * x - 1;
			/* determine whether cse 4 or 5 */
			if(e <= 0){
				fb_circle_moved(&x, &y, &d);
			}else{
				fb_circle_movev(&x, &y, &d);
			}
		}else if(d == 0){
			fb_circle_moved(&x, &y, &d);
		}
	}

	return 0;
}
