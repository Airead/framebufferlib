/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 07 09:46:26 CST			*
 ********************************************************
 *		after studying C 51 days		*
 *		after studying APUE 16 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "image.h"
#include "pixel.h"
#include "screen.h"

#define TEST 0

/*
 * show image at fbp
 */
int fb_image_show(FB *fbp, FB_IMAGE *imagep)
{
	int x, y;
	FB_POINT point;
	COLOR_32 color;

	for(y = 0; y < fbp->fb_vinfo.yres; y++)
		for(x = 0; x < fbp->fb_vinfo.xres; x++){
			if(x < imagep->width && y < imagep->height){
				color = fb_formatRGB(
					imagep->imagestart[(y * imagep->width + x) * imagep->components],
					imagep->imagestart[(y * imagep->width + x) * imagep->components + 1],
					imagep->imagestart[(y * imagep->width + x) * imagep->components + 2]);

				fb_set_pixel(&point, x, y, color);
				fb_draw_pixel(fbp, &point);
			}else{
				color = fb_formatRGB(0, 0, 0);
				fb_set_pixel(&point, x, y, color);
				
				fb_draw_pixel(fbp, &point);
			}
		}

	return 0;
}

/*
 * show image information
 */
int fb_imageinfo(FB_IMAGE *imagep)
{
	fprintf(stdout, "image information:\n");
	fprintf(stdout, "width: %d, height: %d, components: %d\n",
		imagep->width, imagep->height, imagep->components);
	fprintf(stdout, "image size: %lu, image start point: %#08lx\n",
		imagep->imagesize, (unsigned long)imagep->imagestart);

	return 0;
}


/* XXX */
int fb_image_transformm2(FB *fbp, FB_IMAGE *imagep, float hs, float vs)
{
	int i, j;
	int x, y;
	int x1, y1;
	unsigned long location;
	unsigned char *p;
	float ex, ey;
	unsigned char *tmpimagep;
	
	tmpimagep = malloc(fbp->fb_screensize);

	p = tmpimagep;
	x1 = 0;
	y1 = 0;
	i = 0; j = 0;
	ex = -0.5;
	ey = -0.5;
	for(y = 0; y < fbp->fb_vinfo.yres; y++){
		for(x = 0; x < fbp->fb_vinfo.xres; x++){
			if(x < imagep->width * hs && y < imagep->height * vs){
				if(ex < 0){
					ex = ex + 1 / hs;
				}else{
					i++;
					ex = ex - 1;
				}

				if(ey < 0){
					ey = ey + 1 / vs;
				}else{
					j++;
					ey = ey - 1;
				}
//				fprintf(stdout, "i = %d, j = %d\n", i, j);
//				usleep(10000);
				location = (j * imagep->width + i) * imagep->components;

				*p++ = imagep->imagestart[location + 2];
				*p++ = imagep->imagestart[location + 1];
				*p++ = imagep->imagestart[location];
				*p++ = 0x00;
			}else{
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
		}
	}

	//void *memcpy(void *dest, const void *src, size_t n);		
	memcpy(fbp->fb_start, tmpimagep, fbp->fb_screensize);

	free(tmpimagep);
	
	return 0;
}

int fb_image_transformm(FB *fbp, FB_IMAGE *imagep, float hs, float vs)
{
	int i, j;
	int x, y;
	unsigned long location;
	unsigned char *p;
	unsigned char *tmpimagep;
	
	tmpimagep = malloc(fbp->fb_screensize);

	p = tmpimagep;

	for(y = 0; y < fbp->fb_vinfo.yres; y++){
		for(x = 0; x < fbp->fb_vinfo.xres; x++){
			if(x < imagep->width * hs && y < imagep->height * vs){
				i = x / hs;
				j = y / vs;
			
				location = (j * imagep->width + i) * imagep->components;

				*p++ = imagep->imagestart[location + 2];
				*p++ = imagep->imagestart[location + 1];
				*p++ = imagep->imagestart[location];
				*p++ = 0x00;
			}else{
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
				*p++ = 0;
			}
		}
	}

	//void *memcpy(void *dest, const void *src, size_t n);		
	memcpy(fbp->fb_start, tmpimagep, fbp->fb_screensize);

	free(tmpimagep);
	
	return 0;
}

/* FIXME */
int fb_image_transform(FB *fbp, FB_IMAGE *imagep, float hs, float vs)
{
	int i, j;
	int x, y;
	FB_POINT point;
	unsigned long location;
	COLOR_32 color;
	
	for(y = 0; y < imagep->height * vs; y++){
		for(x = 0; x < imagep->width * hs; x++){
			i = x / hs;
			j = y / vs;
			
			location = (j * imagep->width + i) * imagep->components;
			
			color = fb_formatRGB(
				imagep->imagestart[location],
				imagep->imagestart[location + 1],
				imagep->imagestart[location + 2]);
			
			fb_set_pixel(&point, x, y, color);
			fb_draw_pixel(fbp, &point);
		}
	}
	
	return 0;
}

/*
 * Rotary image
 */
int fb_image_rotate(FB_IMAGE *imagep, FB_IMAGE *retimgp, float radian)
{
	int i, j, k;
	int nx, ny;
	int centerx, centery;
        unsigned long img_loc, ret_loc;
	
	retimgp->components = imagep->components;
	retimgp->width = sqrt(imagep->width * imagep->width + imagep->height * imagep->height);
       	retimgp->height = retimgp->width;
	retimgp->imagesize = retimgp->width * retimgp->height * retimgp->components;
	retimgp->x = imagep->x;
	retimgp->y = imagep->y;
	retimgp->imagestart = malloc(sizeof(char) * retimgp->imagesize);

	memset(retimgp->imagestart, 0, retimgp->imagesize);

	centerx = imagep->width / 2;
	centery = imagep->height / 2;
	
	for(i = 0; i < imagep->height; i++){
		for(j = 0; j < imagep->width; j++){
			nx = (j - centerx) * cos(radian) - (i - centery) * sin(radian) + centerx + (retimgp->width - imagep->width) / 2;
			ny = (j - centerx) * sin(radian) + (i - centery) * cos(radian) + centery + (retimgp->height - imagep->height) / 2;
			ret_loc = (ny * retimgp->width + nx) * retimgp->components;
			img_loc = (i * imagep->width + j) * imagep->components;
			if(0 <= nx && nx < retimgp->width && 0 <= ny && ny <= retimgp->height){
				for(k = 0; k < 4; k++){
					retimgp->imagestart[ret_loc + k] = imagep->imagestart[img_loc + k];
				}
			}
		}
	}

#if TEST
	fprintf(stdout, "----------------------------------------\n");
	fprintf(stdout, "nx: %d, ny: %d, imgloc: %lu, retloc: %lu\n", nx, ny, img_loc, ret_loc);
	fprintf(stdout, "radian = %f, nwidth = %d, height = %d, retsize = %lu\n",radian, retimgp->width, retimgp->height, retimgp->imagesize);
	fprintf(stdout, "radian = %f, nwidth = %d, height = %d, imgsize = %lu\n",radian, imagep->width, imagep->height, imagep->imagesize);
#endif
	
	return 0;
}


/*
 * Rotary image dest form source
 */
int fb_image_rotate2(FB_IMAGE *imagep, FB_IMAGE *retimgp, float radian)
{
	int i, j, k;
	int ax, ay;
	int ox, oy;
	int h, l, centerx, centery;
        unsigned long img_loc, ret_loc;
	unsigned long adjust;

	ax = (retimgp->width - imagep->width) / 2;
	ay = (retimgp->height - imagep->height) / 2;

	
	retimgp->components = imagep->components;
	retimgp->width = sqrt(imagep->width * imagep->width + imagep->height * imagep->height);
       	retimgp->height = retimgp->width;
	retimgp->imagesize = retimgp->width * retimgp->height * retimgp->components;
	retimgp->x = imagep->x - ax;
	retimgp->y = imagep->y - ay;
	retimgp->imagestart = malloc(sizeof(char) * retimgp->imagesize);

	memset(retimgp->imagestart, 0, retimgp->imagesize);

	h = centerx = retimgp->width / 2;
	l = centery = retimgp->height / 2;
	adjust = (ay * retimgp->width + ax) * imagep->components;
	
	for(i = 0; i < retimgp->height; i++){
		for(j = 0; j < retimgp->width; j++){
			ox = (j - h) * cos(radian) + (i - l) * sin(radian) + h - ax;
			oy = (i - l) * cos(radian) - (j - h) * sin(radian) + l - ay
;

			//ox = j * cos(radian) + i * sin(radian);
			//oy = i * cos(radian) - j * sin(radian);
			

			ret_loc = (i * retimgp->width + j) * retimgp->components;
			img_loc = (oy * imagep->width + ox) * imagep->components;
			if(i < retimgp->height && j  < retimgp->width){
				if(0 <= ox && ox < imagep->width && 0 <= oy && oy <= imagep->height){
					for(k = 0; k < 4; k++){
						retimgp->imagestart[ret_loc + k] = imagep->imagestart[img_loc + k];
					}
				}
			}
		}
	}
	
#if TEST
	fprintf(stdout, "----------------------------------------\n");
	fprintf(stdout, "ox: %d, oy: %d, imgloc: %lu, retloc: %lu\n", ox, oy, img_loc, ret_loc);
	fprintf(stdout, "ret: radian = %f, nwidth = %d, height = %d, retsize = %lu\n",radian, retimgp->width, retimgp->height, retimgp->imagesize);
	fprintf(stdout, "img: radian = %f, nwidth = %d, height = %d, imgsize = %lu\n",radian, imagep->width, imagep->height, imagep->imagesize);
#endif
	
	return 0;
}


/*
 * Destroy a iamge
 */
int fb_image_destory(FB_IMAGE *imagep)
{
	free(imagep->imagestart);

	return 0;
}

/*
 * enlarge a image
 */
int fb_image_enlarge(FB_IMAGE *imagep, FB_IMAGE *retimgp, float proportionx, float proportiony)
{
	int i, j, k;
	int nx, ny;
	unsigned long tm_loc, loc;
	
	retimgp->components = imagep->components;
	retimgp->width = imagep->width * proportionx;
	retimgp->height = imagep->height * proportiony;
	retimgp->imagesize = retimgp->width * retimgp->height * retimgp->components;
	retimgp->proportionx = proportionx;
	retimgp->proportiony = proportiony;
	retimgp->x = imagep->x;
	retimgp->y = imagep->y;
	retimgp->imagestart = malloc(sizeof(char) * retimgp->imagesize);
	
	for(i = 0; i < retimgp->height; i++){
		for(j = 0; j < retimgp->width; j++){
			nx = j / proportionx;
			ny = i / proportiony;
			loc = (ny * imagep->width + nx) * retimgp->components;
			tm_loc = (i * retimgp->width + j) * retimgp->components;
			for(k = 0; k < 4; k++){
				retimgp->imagestart[tm_loc + k] = imagep->imagestart[loc + k];
			}
		}
	}
	

	return 0;
}

/*
 * set retimgp center
 */
int fb_image_entlage_setcenter(FB_IMAGE *imagep, FB_IMAGE *retimgp)
{
	retimgp->x = imagep->x - (imagep->width / 2) * retimgp->proportionx;
	retimgp->y = imagep->y - (imagep->height / 2) * retimgp->proportiony;

	return 0;
}

/*
 * set image display position
 */
int fb_image_setpos(FB_IMAGE *imagep, int x, int y)
{
	imagep->x = x;
	imagep->y = y;

	return 0;
}

