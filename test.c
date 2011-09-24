/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 19:40:52 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "framebuffer.h"
#include "pixel.h"
#include "plane.h"
#include "line.h"
#include "circle.h"

#define PLANE_PHOTO_FRAME 1
#define PLANE_ROTATION 0
#define PLANE_TRANSFORM 0
#define LINE 0
#define CIRCLE 0
int main(int argc, char *argv[])
{
	FB fb;
	FB_RECT rect;
	COLOR_32 color;
	FB_POINT point1, point2;
	int x, y;
	float radian;
	
	//unsigned char *fb_open(char *fbname, struct framebuffer *fbp)
	fb_open("/dev/fb0", &fb);
	fb_rect_draw_background(&fb, 0x0);

	while(1){
#if PLANE_PHOTO_FRAME
		int i, j, k;
		int w, h, t;
		int tmp;
		static int select;
		
		i = 400;
		j = 300;
		w = 100;
		h = 100;
		t = 6;
		
		select++;
		for(k = 0; k < t; k+=2){
			if(select % 2 == 0){
				tmp = 50 + (255 - 50) / t * k;
			}else{
				tmp = 190 + (255 - 190) / t * k;
			}
			color = fb_formatRGB(tmp, tmp, tmp);	
			fb_set_pixel(&point1, i - k/2, j - k/2, color);
			fb_rect_set(&rect, &point1, w + k, h + k);
			//int fb_rect_draw_nonfill(FB *fbp, FB_RECT *rectp);
			fb_rect_draw_nofill(&fb, &rect);
		}
#if 0
		fb_set_pixel(&point1, i+1, j+1, 0x777777);
		fb_rect_set(&rect, &point1, w+1, h+1);
		//int fb_rect_draw_nonfill(FB *fbp, FB_RECT *rectp);
		fb_rect_draw_nofill(&fb, &rect);

		fb_set_pixel(&point1, i+2, j+2, 0xffffff);
		fb_rect_set(&rect, &point1, w+2, h+2);
		//int fb_rect_draw_nonfill(FB *fbp, FB_RECT *rectp);
		fb_rect_draw_nofill(&fb, &rect);
#endif	
#endif

#if PLANE_TRANSFORM
		fb_set_pixel(&point1, 400, 300, 0x0);
		fb_rect_set(&rect, &point1, 100, 100);
		fb_set_pixel(&point2, 400, 300, 0);
		
		fb_rect_draw(&fb, &rect);
		//int fb_rect_tranform(FB *fbp, FB_RECT *fb_rectp, int flag, float hs, float vs, FB_POINT *fixpoint)
//		fb_rect_tranform(&fb, &rect, FB_ROTATION_FIXPOINT, 1, 1, &point2);
#endif

#if PLANE_ROTATION
		/* draw rect */

		fb_set_pixel(&point1, 400, 300, 0x777777);
		fb_rect_set(&rect, &point1, 100, 100);
		fb_set_pixel(&point2, 400, 300, 0);

		radian = 0;
	        while(1){
			fb_draw_background(&fb, 0xffffff);
			fb_rect_rotation(&fb, &rect, radian, FB_ROTATION_FIXPOINT, &point2);
			radian = radian + 0.1;
			usleep(1000);
		}
#endif
		
#if LINE
		fb_set_pixel(&point1, 400, 300, 0);
		/* draw line */
		for(x = 0; x < 800; x+=10){
			fb_set_pixel(&point2, x, 0, 0);
			fb_draw_line(&fb, &point1, &point2);
		}

		/* draw line */
		for(x = 0; x < 800; x+=10){
			fb_set_pixel(&point2, x, 599, 0);
			fb_draw_line(&fb, &point1, &point2);
		}

		/* draw line */
		for(y = 0; y < 600; y+=10){
			fb_set_pixel(&point2, 0, y, 0);
			fb_draw_line(&fb, &point1, &point2);
		}
		/* draw line */

		for(y = 0; y < 600; y+=10){
			fb_set_pixel(&point2, 799, y, 0);
			fb_draw_line(&fb, &point1, &point2);
		}
		fb_set_pixel(&point2, 799, 599, 0);
		fb_draw_line(&fb, &point1, &point2);
#endif		

#if CIRCLE
		for(y = 0; y < 299; y++){
			color = formatRGB((y / 299.) * 255, (y / 299.) * 255, (y / 299.) * 255);
			fb_set_pixel(&point1, 400, 300, color);
			fb_draw_circle(&fb, &point1, y);
		}
#endif
		
		sleep(2);
		usleep(1000);
	}


	//step 3. close 
	fb_close(&fb);
	
	return 0;
}

