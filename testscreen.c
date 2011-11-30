/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 07 15:35:32 CST			*
 ********************************************************
 *		after studying C 51 days		*
 *		after studying APUE 16 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "screen.h"
#include "image.h"
#include "jpeg.h"

#define MOVE 1
#define LARGE 0
#define ROTA 0

int main(int argc, char *argv[])
{
	FB fb;
	FB_SCREEN screen;
	FB_IMAGE image;
	FB_IMAGE background;
	FB_IMAGE retimg;

	//int fb_open(char *fbname, FB *fbp);
	fb_open("/dev/fb0", &fb);

	//int fb_screen_init(FB_SCREEN *screenp, FB *fbp);
	fb_screen_init(&screen, &fb);

	//int fb_screen_info(FB_SCREEN *screenp)
	fb_screen_info(&screen);

	//int fb_load_jpeg(FB_IMAGE *imagep, char *filename);
	fb_load_jpeg(&image, "logo.jpg");
	fb_load_jpeg(&background, "taiji_logo.jpg");

	fprintf(stdout, "Before opt, image size: %lu\n", image.imagesize);
	fb_imageinfo(&image);

	/* test function before here */
	//int fb_image_show(FB *fbp, FB_IMAGE *image);
//	fb_image_show(&fb, &image);

	//int fb_screen_optimize_image(FB_SCREEN *screenp, FB_IMAGE imagep);
	fb_screen_optimize_image(&screen, &image);
	fb_screen_optimize_image(&screen, &background);
	
	fprintf(stdout,"----------------------------------------\n");
	fprintf(stdout, "After opt, image size: %lu\n", image.imagesize);
	fb_imageinfo(&image);

#if MOVE
	int i;

	while(1){
		for(i = -300; i < 800; i += 2){
			//int fb_screen_add_image(FB_SCREEN *screenp, int x, int y, FB_IMAGE *imagep)
			fb_image_setpos(&background, -120, -110);
			fb_screen_add_image(&screen, &background);
			
			fb_image_setpos(&image, i, 300);
			fb_screen_add_image(&screen, &image);

			fb_image_setpos(&image, 200, i);
			fb_screen_add_image(&screen, &image);

			fb_image_setpos(&image, i, 50);
			
			fb_screen_add_image(&screen, &image);
			usleep(20000);
	
			fb_screen_update(&screen);
		}
	
		for(; i > -300; i -= 2){
			//int fb_screen_add_image(FB_SCREEN *screenp, int x, int y, FB_IMAGE *imagep)
			fb_image_setpos(&background, -120, -110);
			fb_screen_add_image(&screen, &background);
			
			fb_image_setpos(&image, i, 300);
			fb_screen_add_image(&screen, &image);

			fb_image_setpos(&image, 200, i);
			fb_screen_add_image(&screen, &image);

			fb_image_setpos(&image, i, 50);
			
			fb_screen_add_image(&screen, &image);
			usleep(20000);
	
			fb_screen_update(&screen);
		}

	}
#endif

#if LARGE
	float px, py;

	//int fb_image_setpos(FB_IMAGE *imagep, int x, int y);
	fb_image_setpos(&image, 400, 300);
	
	px = 0.1;
	while(px < 5){
		//int fb_screen_add_image_enlarge(FB_SCREEN *screenp, FB_IMAGE *imagep, int x, int y,
		//                                float proportionx, float proportiony);
		//fb_screen_add_image_enlarge(&screen, &image, px, px);


		//int fb_image_enlarge(FB_IMAGE *imagep, FB_IMAGE *retimgp, 
		//                     float proportionx, float proportiony)
		fb_image_enlarge(&image, &retimg, px, px);

		fb_image_setpos(&image, 200, 200);
		//int fb_image_entlage_setcenter(FB_IMAGE *image, FB_IMAGE *retimgp);
		fb_image_entlage_setcenter(&image, &retimg);
		fb_screen_add_image(&screen, &retimg);

		fb_image_setpos(&image, 600, 300);
		//int fb_image_entlage_setcenter(FB_IMAGE *image, FB_IMAGE *retimgp);
		fb_image_entlage_setcenter(&image, &retimg);
		fb_screen_add_image(&screen, &retimg);

		fb_image_setpos(&image, 500, 400);
		//int fb_image_entlage_setcenter(FB_IMAGE *image, FB_IMAGE *retimgp);
		fb_image_entlage_setcenter(&image, &retimg);
		fb_screen_add_image(&screen, &retimg);

		//int fb_screen_add_image(FB_SCREEN *screenp, int x, int y, FB_IMAGE *imagep)
		fb_screen_add_image(&screen, &retimg);

		fb_screen_update(&screen);

		fb_image_destory(&retimg); /* destory */

		px = px * 1.02;
		usleep(20000);
	}
#endif

#ifdef ROTA
//	FB_IMAGE retimg;
	float radian;

	fb_image_setpos(&image, 200, 200);
	fb_image_setpos(&background, 145, 45);
	
radian = 0;
	while(radian > -1){

//		fb_screen_clear(&screen);

		//int fb_image_rotate(FB_IMAGE *imagep, FB_IMAGE *retimgp, float radian)
//		fb_image_rotate2(&image, &retimg, radian);
		fb_image_rotate2(&background, &retimg, radian);

		fb_screen_add_image(&screen, &retimg);

		usleep(40000);
		fb_screen_update(&screen);
	
		fb_image_destory(&retimg);

		radian += 0.005;
	}
	
#endif
	//int fb_image_destory(FB_IMAGE *imagep);
	fb_image_destory(&image);
	fb_image_destory(&background);

	//int fb_screen_destory(FB_SCREEN *screenp);
	fb_screen_destory(&screen);

	return 0;
}
