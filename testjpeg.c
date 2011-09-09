/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 02 15:43:49 CST			*
 ********************************************************
 *		after studying C 46 days		*
 *		after studying APUE 11 days		*
 ********************************************************/

/*
 * This program demonstrates framebuffer
 * draw some pixel in screen
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <jpeglib.h>
#include <sys/ioctl.h>
#include "framebuffer.h"
#include "image.h"
#include "jpeg.h"
#include "pixel.h"
#include "plane.h"

#define DYT 0
#define ROTA 0

unsigned char *raw_image = NULL;

int main(int argc, char *argv[])
{

	float hs, vs;

	if(argc < 2){
		fprintf(stderr, "usage: %s <jpgname>\n", argv[0]);
		exit(1);
	}else if(argc == 2){
		hs = 1;
		vs = 1;
	}else{
		//float strtof(const char *nptr, char **endptr);
		hs = strtof(argv[2], NULL);
		vs = strtof(argv[3], NULL);
	}
	
	FB_IMAGE image;

	//unsigned char *fb_load_jpeg(FB_IMAGE *imagep, char *filename);
	fb_load_jpeg(&image, argv[1]);

	
	/* initial frame buffer */
	FB fb;

	//unsigned char *fb_open(char *fbname, struct framebuffer *fbp)
	if(fb_open("/dev/fb0", &fb) < 0){
		fprintf(stderr, "fb_open failed");
		exit(1);
	}

	fb_show_imageinfo(&image);

#if ROTA
	hs = 0;
	while(1){
		//int fb_show_image_rotate(FB *fbp, FB_IMAGE *imagep, float radian)
		fb_show_image_rotate(&fb, &image, hs);
		hs += 0.01; 
		usleep(10000);
	}
#endif
	//int fb_draw_background(FB *fbp, COLOR_32 color)
//	fb_draw_background(&fb, 0);
	
//	fb_show_image_transformm(&fb, &image, 1, 1);

#if DYT
	hs = 0; vs = 0;
	while(hs < fb.fb_vinfo.width){
		//int fb_rect_tranform(FB *fbp, FB_RECT *fb_rectp, int flag, float hs, float vs, FB_POINT *fixpoint)
		fb_show_image_transformm(&fb, &image, hs, vs);
		hs += 0.02;
		vs += 0.02;
		usleep(10000);
		
	}
#endif
	//int fb_close(struct framebuffer *fbp);
	if(fb_close(&fb) < 0){
		fprintf(stderr, "fb_close failed\n");
		exit(1);
	}

	return 0;
}

