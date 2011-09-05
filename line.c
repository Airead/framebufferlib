/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 22:48:14 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "framebuffer.h"
#include "pixel.h"
#include "line.h"

#define TEST 0

/*
 * The sign function returns -1, 0, 1,
 * as its argument is <0, =0, or >0
 */
int fb_line_sign(int x)
{
	if(x < 0){
		return -1;
	}else if(x == 0){
		return 0;	
	}else{
		return 1;
	}
}

/*
 * Draw a line form point1 to point2.
 */
int fb_draw_line(FB *fbp, FB_POINT *point1, FB_POINT *point2)
{
	/* generalized integer Bresenham's algorithm for all quadrants
	 * the line end points aire (x1, y1) and (x2 , y2), assumed not equal
	 * all variables are assumed integer*/
	/* initialize variables */
	FB_POINT point;
	int x, y, dx, dy;
	int s1, s2;
	int e;			/* err(math) */
	int i, tmp, interchange;

	x = point1->x;
	y = point1->y;
	dx = abs(point2->x - x);
	dy = abs(point2->y - y);
	s1 = fb_line_sign(point2->x - point1->x);
	s2 = fb_line_sign(point2->y - point1->y);
	
	/* interchange dx and dy depending on the slope of the line */
	if(dy > dx){
		tmp = dx;
		dx = dy;
		dy = tmp;
		interchange = 1;
	}else{
		interchange = 0;
	}

	/* initialize the error term to compensate for a non zero intercept */
	e = 2 * dy - dx;

	/* main loop */
	for(i = 0; i < dx; i++){
		fb_set_pixel(&point, x, y, point1->color);
		fb_draw_pixel(fbp, &point);
		while(e > 0){
			if(interchange == 1){
				x = x + s1;
			}else{
				y = y + s2;
			}
			e = e - 2 * dx;
		}

		if(interchange == 1){
			y = y + s2;
		}else{
			x = x + s1;
		}
		e = e + 2 * dy;
	}

	return 0;
}

/*
 * Draw a line form point1 to point2, only for the first octant
 */
int fb_draw_line_study(FB *fbp, FB_POINT *point1, FB_POINT *point2)
{
	/* Bresenham's line rasterization algorithm for the first octant
	 * the line end points are (x1, y1) and (x2, y2), assumed not equal
	 * Interger is the interger function*/
	FB_POINT point;
	int x, y, dx, dy;
	float m, e;
	int i;

	/* initialize variables */
	x = point1->x;
	y = point1->y;
	dx = point2->x - point1->x;
	dy = point2->y - point1->y;
	m = dy / (float)dx;

	/* initialize e to compensate for a nonzero intercept */
	e = m - 0.5;

#if TEST
	fprintf(stdout, "initialize variables: \n");
	fprintf(stdout, "(x, y) = (%d, %d)\n", x, y);
	fprintf(stdout, "(dx, dy) = (%d, %d)\n", dx, dy);
	fprintf(stdout, "m = %f, e = %f\n", m, e);
#endif
	/* begin the main loop */
	for(i = 0; i < dx; i++){
		fb_set_pixel(&point, x, y, point1->color);
		fb_draw_pixel(fbp, &point);
		while(e > 0){
			y = y + 1;
			e = e - 1;
		}
		x = x + 1;
		e = e + m;
	}

	return 0;
}
