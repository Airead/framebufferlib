/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 07 15:21:53 CST			*
 ********************************************************
 *		after studying C 51 days		*
 *		after studying APUE 16 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "image.h"
#include "pixel.h"
#include "screen.h"
#include "framebuffer.h"

/*
 * Initialize screen according framebuffer
 */
int fb_screen_init(FB_SCREEN *screenp, FB *fbp) 
{
	screenp->width = fbp->fb_vinfo.xres;
	screenp->height = fbp->fb_vinfo.yres;
	screenp->screensize = fbp->fb_screensize;
	screenp->fb_start = fbp->fb_start;
	screenp->pixelbits = fbp->fb_vinfo.bits_per_pixel;
	screenp->screenstart = malloc(sizeof(char) * screenp->screensize);
	
	return 0;
}

/*
 * Destory screen
 */
int fb_screen_destory(FB_SCREEN *screenp)
{
	free(screenp->screenstart);
 
	return 0;
}

/*
 * print screen information
 */
int fb_screen_info(FB_SCREEN *screenp)
{
	fprintf(stdout,"----------------------------------------\n");
	fprintf(stdout, "screen information\n");
	fprintf(stdout, "width: %d\n", screenp->width);
	fprintf(stdout, "height: %d\n", screenp->height);
	fprintf(stdout, "screensize: %lu\n", screenp->screensize);
	fprintf(stdout, "pixel bits: %lu\n", screenp->pixelbits);
	fprintf(stdout, "fb_start: %#08lx\n", 
		(unsigned long)screenp->fb_start);
	fprintf(stdout, "screenstart: %#08lx\n", 
				(unsigned long)screenp->screenstart);
	fprintf(stdout,"----------------------------------------\n");
	
	return 0;
}

/*
 * Fill screen with black
 */
int fb_screen_clear(FB_SCREEN *screenp)
{
	memset(screenp->screenstart, 0xff, screenp->screensize);

	return 0;
}

/*
 * add image to screen at (x, y)
 * assume that image always is rectangle
 */
int fb_screen_add_image(FB_SCREEN *screenp, FB_IMAGE *imagep)
{
	int j;
	unsigned long sc_loc;	/* screen location */
	unsigned long im_loc;	/* image location */
	long cpbytes;
	unsigned char *p, *q;

	p = screenp->screenstart;
	q = imagep->imagestart;

	
	for(j = 0; j < imagep->height; j++){
		/* vertical overflow check */
		if(0 < imagep->y + j && imagep->y + j < screenp->height){
			sc_loc = ((imagep->y + j) * screenp->width + imagep->x) * screenp->pixelbits / 8;
			im_loc = j * imagep->width * imagep->components;
#if TEST		
			if(j > 200){
				fprintf(stdout, "imagesize = %lu, screensize = %lu\n", 
					imagep->imagesize, screenp->screensize);
				fprintf(stdout, "j = %d, sc_loc = %lu, im_loc = %lu\n\n", j, sc_loc, im_loc);
				usleep(50000);
			}
#endif
			/* horizontal overflow check */
			if(imagep->x >= 0){
				if(screenp->width - imagep->x < imagep->width){
					cpbytes = screenp->width - imagep->x;
				}else{
					cpbytes = imagep->width;
				}
			}else{
				sc_loc = sc_loc - imagep->x * 4; /* -x > 0 */
				im_loc = im_loc - imagep->x * 4;
				cpbytes = imagep->width + imagep->x;
			}
			//void *memcpy(void *dest, const void *src, size_t n);
			if(cpbytes > 0){
				memcpy(p + sc_loc, q + im_loc, cpbytes * 4); 
			}
		}
	}

	return 0;
}

/*
 * optimize image to 32 bits per pixel
 * now only fit 32bits
 */
int fb_screen_optimize_image(FB_SCREEN *screenp, FB_IMAGE *imagep)
{
	unsigned char *image_opt;
	unsigned char *p, *q;
	unsigned long optsize;
	int i;

	/* update */ 
	optsize = imagep->width * imagep->height * screenp->pixelbits / 8;
	
	image_opt = malloc(sizeof(char) * optsize);

	if(image_opt == NULL){
		fprintf(stderr, "%s:%d: malloc failed: %s\n",
			__FILE__, __LINE__, strerror(errno));
		
		return -1;
	}

	i = 0;
	p = image_opt; q = imagep->imagestart;
	while(i < optsize){
		p[i + 3] = 0;	      /* transp */
		p[i + 2] = *q++;      /* red */  
		p[i + 1] = *q++;      /* green */
		p[i] = *q++;	      /* blue */

		i += 4;
	}
	
	/* update image information */
	imagep->components = screenp->pixelbits / 8;
	imagep->imagesize = optsize;
	free(imagep->imagestart); /* free old image memory */
	imagep->imagestart = image_opt;

	//CHECK MALLOC
	return 0;
}

/*
 * update screen
 */
int fb_screen_update(FB_SCREEN *screenp)
{
	memcpy(screenp->fb_start, screenp->screenstart, screenp->screensize);

	return 0;
}

/*
 * Change image large or small
 */
int fb_screen_add_image_enlarge(FB_SCREEN *screenp, FB_IMAGE *imagep,
				float proportionx, float proportiony) 
{
	FB_IMAGE tmpimg;
	int i, j, k;
	int nx, ny;
	unsigned long tm_loc, loc;
	
	
	tmpimg.components = screenp->pixelbits / 8;
	tmpimg.width = imagep->width * proportionx;
	tmpimg.height = imagep->height * proportiony;
	
	tmpimg.imagestart = malloc(tmpimg.width * tmpimg.height * tmpimg.components);
	
	for(i = 0; i < tmpimg.height; i++){
		for(j = 0; j < tmpimg.width; j++){
			nx = j / proportionx;
			ny = i / proportiony;
			loc = (ny * imagep->width + nx) * tmpimg.components;
			tm_loc = (i * tmpimg.width + j) * tmpimg.components;
			for(k = 0; k < 4; k++){
				tmpimg.imagestart[tm_loc + k] = imagep->imagestart[loc + k];
			}
		}
	}

	tmpimg.x = imagep->x - (imagep->width / 2) * proportionx;
	tmpimg.y = imagep->y - (imagep->height / 2) * proportiony;

	//int fb_screen_add_image(FB_SCREEN *screenp, int x, int y, FB_IMAGE *imagep)
	fb_screen_add_image(screenp, &tmpimg);
	
	fb_image_destory(&tmpimg);

	return 0;
}

#if 0
/*
 * rotate image
 */
int fb_screen_add_image_rotate(FB_SCREEN *screenp, FB_IMAGE *imagep, float radian)
{
	int i, j;
	COLOR_32 *sp;
	COLOR_32 *ip;

	sp = (COLOR_32 *)screenp->screenstart;
	ip = (COLOR_32 *)imagep->imagestart;

	for(i = 0; i < imagep->height; i++){
		for(j = 0; j < imagep->width; j++){
			//convert raster to Cartesian
			x = j - icenterx;
			y = icentery - i;

			//convert Cartesian to polar
			fDistance = sqrt(x * x + y * y);
			fPolarangle = 0.0;
			
			if(x == 0){
				if(y == 0){
					//centre of image, no rotation needed
					sp[y * imagep->width + x] = 0;
				}
			}
		}
	}
}
#endif
