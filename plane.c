/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 21:31:07 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "pixel.h"
#include "line.h"
#include "plane.h"
#include "framebuffer.h"


int fb_rect_set(FB_RECT *fb_rectp, FB_POINT *point, int width, int height)
{
	fb_rectp->left_top.x = point->x;
	fb_rectp->left_top.y = point->y;
	fb_rectp->left_top.color = point->color;
	fb_rectp->width = width;
	fb_rectp->height = height;

	return 0;
}

/* 
 * set background with color
 */
int fb_rect_draw_background(FB *fbp, COLOR_32 color)
{
	int x, y;
	FB_POINT point;

	point.color = color;

	for(y = 0; y < fbp->fb_vinfo.yres; y++){
		for(x = 0; x < fbp->fb_vinfo.xres; x++){
			point.x = x;
			point.y = y;
			fb_draw_pixel(fbp, &point);
		}
	}
	
	return 0;
}

/*
 * draw a rect from left_top to right_bottom
 * color according left_top
 */
int fb_rect_draw(FB *fbp, FB_RECT *fb_rectp) 
{
	fb_rect_rotation(fbp, fb_rectp, 0, FB_ROTATION_ORIGIN, NULL);

	return 0;
}

/*
 * draw a rect from left_top to right_bottom
 * color according left_top
 */
int fb_rect_draw_nofill(FB *fbp, FB_RECT *rectp) 
{
	FB_POINT point1, point2;

	/* top */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y, rectp->left_top.color);
	//int fb_draw_line(FB *fbp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line(fbp, &point1, &point2);

	/* left */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x, rectp->left_top.y + rectp->height, 
		     rectp->left_top.color);
	//int fb_draw_line(FB *fbp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line(fbp, &point1, &point2);

	/* bottom */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y + rectp->height - 1, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y + rectp->height - 1, rectp->left_top.color);
	//int fb_draw_line(FB *fbp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line(fbp, &point1, &point2);

	/* right */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x + rectp->width - 1, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y + rectp->height - 1, rectp->left_top.color);
	//int fb_draw_line(FB *fbp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line(fbp, &point1, &point2);
	

	return 0;
}

/*
 * draw a rect from left_top to right_bottom
 * color according left_top
 */
int fb_rect_draw_nofill_screen(FB_SCREEN *screenp, FB_RECT *rectp) 
{
	FB_POINT point1, point2;

	/* top */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y, rectp->left_top.color);
	//int fb_draw_line_screen(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen(screenp, &point1, &point2);

	/* left */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x, rectp->left_top.y + rectp->height, 
		     rectp->left_top.color);
	//int fb_draw_line_screen(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen(screenp, &point1, &point2);

	/* bottom */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y + rectp->height - 1, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y + rectp->height - 1, rectp->left_top.color);
	//int fb_draw_line_screen(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen(screenp, &point1, &point2);

	/* right */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x + rectp->width - 1, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y + rectp->height - 1, rectp->left_top.color);
	//int fb_draw_line_screen(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen(screenp, &point1, &point2);
	

	return 0;
}

/*
 * rotation a rect.
 * @radian: angle
 * @flag: FB_ROTATION_CENTER | FB_ROTATION_FIXPOINT | FB_ROTATION_ORIGIN
 * @fixpoint: */
int fb_rect_rotation(FB *fbp, FB_RECT *fb_rectp, double radian, int flag, FB_POINT *fixpoint)
{
	int x, y;
	int h, k;		/* (h, k) center of rectangle */
	FB_POINT point;

	point.color = fb_rectp->left_top.color;
	if(flag == FB_ROTATION_CENTER){
		h =  fb_rectp->left_top.x + fb_rectp->width / 2;
		k =  fb_rectp->left_top.y + fb_rectp->height / 2;
	}else if(flag == FB_ROTATION_FIXPOINT){
		h = fixpoint->x;
		k = fixpoint->y;
	}else if(flag == FB_ROTATION_ORIGIN){
		;
	}else{
		fprintf(stderr, "%s:%c: invail argument:\n", __FILE__, __LINE__);
		exit(1);
	}
	
	for(x = fb_rectp->left_top.x; x < fb_rectp->left_top.x + fb_rectp->width; x++){
		for(y = fb_rectp->left_top.y; y < fb_rectp->left_top.y + fb_rectp->height; y++){
			if(flag == FB_ROTATION_FIXPOINT || flag == FB_ROTATION_CENTER){
				point.x = (x - h) * cos(radian) - (y - k) * sin(radian) + h;
				point.y = (x - h) * sin(radian) + (y - k) * cos(radian) + k;
			}else{
				point.x = x * cos(radian) - y * sin(radian);
				point.y = x * sin(radian) + y * cos(radian);
			}
			fb_draw_pixel(fbp, &point);
		}
	}

	return 0;
}

int fb_rect_tranform(FB *fbp, FB_RECT *fb_rectp, int flag, float hs, float vs, FB_POINT *fixpoint)
{
	int x, y;
	int h, k;		/* (h, k) center of rectangle */
	FB_POINT point;

	point.color = fb_rectp->left_top.color;
	if(flag == FB_ROTATION_CENTER){
		h =  fb_rectp->left_top.x + fb_rectp->width / 2;
		k =  fb_rectp->left_top.y + fb_rectp->height / 2;
	}else if(flag == FB_ROTATION_FIXPOINT){
		h = fixpoint->x;
		k = fixpoint->y;
	}else if(flag == FB_ROTATION_ORIGIN){
		;
	}else{
		fprintf(stderr, "%s:%c: invail argument:\n", __FILE__, __LINE__);
		exit(1);
	}
	
	for(x = fb_rectp->left_top.x; x < fb_rectp->left_top.x + fb_rectp->width; x++){
		for(y = fb_rectp->left_top.y; y < fb_rectp->left_top.y + fb_rectp->height; y++){
			if(flag == FB_ROTATION_FIXPOINT || flag == FB_ROTATION_CENTER){
				point.x = hs * (x - h) + h;
				point.y = vs * (y - k) + k;
			}else{
				point.x = hs * x;
				point.y = vs * y;
			}
			fb_draw_pixel(fbp, &point);
		}
	}

	return 0;
}

/*
 * rotation a rect.
 * @radian: angle
 * @flag: FB_ROTATION_CENTER | FB_ROTATION_FIXPOINT | FB_ROTATION_ORIGIN
 * @fixpoint: */
int fb_rect_rotation_screen_trans(FB_SCREEN *screenp, FB_RECT *fb_rectp, double radian, 
				  int flag, FB_POINT *fixpoint, unsigned char trans)
{
	int x, y;
	int h, k;		/* (h, k) center of rectangle */
	FB_POINT point;

	point.color = fb_rectp->left_top.color;
	if(flag == FB_ROTATION_CENTER){
		h =  fb_rectp->left_top.x + fb_rectp->width / 2;
		k =  fb_rectp->left_top.y + fb_rectp->height / 2;
	}else if(flag == FB_ROTATION_FIXPOINT){
		h = fixpoint->x;
		k = fixpoint->y;
	}else if(flag == FB_ROTATION_ORIGIN){
		;
	}else{
		fprintf(stderr, "%s:%c: invail argument:\n", __FILE__, __LINE__);
		exit(1);
	}
	
	for(x = fb_rectp->left_top.x; x < fb_rectp->left_top.x + fb_rectp->width; x++){
		for(y = fb_rectp->left_top.y; y < fb_rectp->left_top.y + fb_rectp->height; y++){
			if(flag == FB_ROTATION_FIXPOINT || flag == FB_ROTATION_CENTER){
				point.x = (x - h) * cos(radian) - (y - k) * sin(radian) + h;
				point.y = (x - h) * sin(radian) + (y - k) * cos(radian) + k;
			}else{
//				point.x = x * cos(radian) - y * sin(radian);
//				point.y = x * sin(radian) + y * cos(radian);
				point.x = x;
				point.y = y;
			}

			fb_set_pixel_trans(&point, point.x, point.y, trans);
			fb_draw_pixel_screen_trans(screenp, &point);

			//int fb_draw_pixel_screen_trans(FB_SCREEN *screenp, FB_POINT *point);
			fb_draw_pixel_screen_trans(screenp, &point); 
		}
	}

	return 0;
}

/*
 * draw a rect from left_top to right_bottom
 * color according left_top
 */
int fb_rect_draw_nofill_screen_trans(FB_SCREEN *screenp, FB_RECT *rectp, unsigned char trans) 
{
	FB_POINT point1, point2;

	/* top */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y, rectp->left_top.color);
	//int fb_draw_line_screen_trans(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen_trans(screenp, &point1, &point2, trans);

	/* left */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x, rectp->left_top.y + rectp->height, 
		     rectp->left_top.color);
	//int fb_draw_line_screen_trans(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen_trans(screenp, &point1, &point2, trans);

	/* bottom */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x, rectp->left_top.y + rectp->height - 1, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y + rectp->height - 1, rectp->left_top.color);
	//int fb_draw_line_screen_trans(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen_trans(screenp, &point1, &point2, trans);

	/* right */
	//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color)
	fb_set_pixel(&point1, rectp->left_top.x + rectp->width - 1, rectp->left_top.y, 
		     rectp->left_top.color);
	fb_set_pixel(&point2, rectp->left_top.x + rectp->width - 1, 
		     rectp->left_top.y + rectp->height - 1, rectp->left_top.color);
	//int fb_draw_line_screen_trans(FB *screenp, FB_POINT *point1, FB_POINT *point2)
	fb_draw_line_screen_trans(screenp, &point1, &point2, trans);
	

	return 0;
}
