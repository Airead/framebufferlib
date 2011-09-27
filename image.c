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
 * full a image to a ensure image
 */
int fb_image_full_image(FB_IMAGE *imagep, FB_IMAGE *retimgp, int lock)
{
	int i, j, k;
	int nx, ny;
	int ox, oy;
	unsigned long tm_loc, loc, offset;
	float px, py;

	px = imagep->width / (float)retimgp->width;
	py = imagep->height / (float)retimgp->height;

	if(lock == IMAGE_FULL_LOCK){
		if(px > py){
			py = px;
		}else{
			px = py;
		}
	}

	ox = (retimgp->width - imagep->width / px) / 2;
	oy = (retimgp->height - imagep->height / py) / 2;

	offset = (oy * retimgp->width + ox) * retimgp->components;

	memset(retimgp->imagestart, 0, retimgp->imagesize);

	for(i = 0; i < retimgp->height; i++){
		for(j = 0; j < retimgp->width; j++){
			nx = j * px;
			ny = i * py;
			tm_loc = (i * retimgp->width + j) * retimgp->components + offset;
			if(nx < imagep->width && ny < imagep->height){
				loc = (ny * imagep->width + nx) * retimgp->components;
				for(k = 0; k < 4; k++){
					retimgp->imagestart[tm_loc + k] = imagep->imagestart[loc + k];
				}
			}
		}
	}

	return 0;
}

/*
 * add a image to a ensure image, according to imagep x and y
 */
int fb_image_add_image(FB_IMAGE *imagep, FB_IMAGE *retimgp)
{
	int j;
	unsigned long ret_loc;	/* retimg location */
	unsigned long im_loc;	/* image location */
	long cpbytes;
	unsigned char *p, *q;

	p = retimgp->imagestart;
	q = imagep->imagestart;

	
	for(j = 0; j < imagep->height; j++){
		/* vertical overflow check */
		if(0 < imagep->y + j && imagep->y + j < retimgp->height){
			ret_loc = ((imagep->y + j) * retimgp->width + imagep->x) * retimgp->components;
			im_loc = j * imagep->width * imagep->components;
#if TEST		
			if(j > 200){
				fprintf(stdout, "imagesize = %lu, screensize = %lu\n", 
					imagep->imagesize, retimgp->screensize);
				fprintf(stdout, "j = %d, ret_loc = %lu, im_loc = %lu\n\n", j, ret_loc, im_loc);
				usleep(50000);
			}
#endif
			/* horizontal overflow check */
			if(imagep->x >= 0){
				if(retimgp->width - imagep->x < imagep->width){
					cpbytes = retimgp->width - imagep->x;
				}else{
					cpbytes = imagep->width;
				}
			}else{
				ret_loc = ret_loc - imagep->x * 4; /* -x > 0 */
				im_loc = im_loc - imagep->x * 4;
				cpbytes = imagep->width + imagep->x;
			}
			//void *memcpy(void *dest, const void *src, size_t n);
			if(cpbytes > 0){
				memcpy(p + ret_loc, q + im_loc, cpbytes * 4); 
			}
		}
	}

	return 0;
}

/*
 * set retimgp center
 */
int fb_image_enlarge_setcenter(FB_IMAGE *imagep, FB_IMAGE *retimgp)
{
	retimgp->x = imagep->x - imagep->width / 2 * (retimgp->proportionx - 1);
	retimgp->y = imagep->y - imagep->height / 2 * (retimgp->proportiony - 1);

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


/*
 * Get image's mini mirror
 */
int fb_image_getmini(FB_IMAGE *imagep, FB_IMAGE *minimgp, 
		     int img_width, int img_height)
{
	int i, j, k;
	int x, y, offx, offy;
	float px, py;
	unsigned long mini_loc, img_loc, offset;

	if(imagep->width > img_width && imagep->height > img_height){
		px = img_width / (float)imagep->width;
		py = img_height / (float)imagep->height;
	}else{
		px = 1;
		py = 1;
	}

	if(px > py){
		offx = (img_width - imagep->width * py) / 2;
		offy = 0;

		px = py;
	}else{
		offx = 0;
		offy = (img_height - imagep->height * px) / 2;
		
		py = px;
	}

	minimgp->components = imagep->components;
	minimgp->width = img_width;
	minimgp->height = img_height;
	minimgp->imagesize = minimgp->width * minimgp->height * minimgp->components;
	minimgp->proportionx = px;
	minimgp->proportiony = py;
	minimgp->x = imagep->x;
	minimgp->y = imagep->y;
	minimgp->imagestart = malloc(sizeof(char) * minimgp->imagesize);

	offset = (offy * minimgp->width + offx) * minimgp->components;

	memset(minimgp->imagestart, 0, minimgp->imagesize);

	for(i = 0; i < minimgp->height; i++){
		for(j = 0; j < minimgp->width; j++){
			y = i / py;
			x = j / px;
			if(x < imagep->width && y < imagep->height){
				img_loc = (y * imagep->width + x) * imagep->components;
				mini_loc = (i * minimgp->width + j) * minimgp->components + offset;
				for(k = 0; k < 4; k++){
					minimgp->imagestart[mini_loc + k] = 
						imagep->imagestart[img_loc + k];
				}
			}
		}
	}

	return 0;
}

/*
 * initialize image 
 */
int fb_image_init(FB_IMAGE *imagep, int width, int height, int components)
{
	imagep->x = 0;
	imagep->y = 0;
	imagep->width = width;
	imagep->height = height;
	imagep->components = components;
	imagep->imagesize = width * height * components;
	imagep->imagestart = malloc(imagep->imagesize);

	return 0;
}

/*
 * Add image to iamge by line
 */
int fb_image_add_image_byline(FB_IMAGE *imagep, FB_IMAGE *retimgp, int line)
{
	int j;
	unsigned long ret_loc;	/* retimgp location */
	unsigned long im_loc;	/* image location */
	long cpbytes;
	unsigned char *p, *q;

	p = retimgp->imagestart;
	q = imagep->imagestart;

	j = line - 1;
	
	/* vertical overflow check */
	if(0 < imagep->y + j && imagep->y + j < retimgp->height){
		ret_loc = ((imagep->y + j) * retimgp->width + imagep->x) * retimgp->components;
		im_loc = j * imagep->width * imagep->components;

		/* horizontal overflow check */
		if(imagep->x >= 0){
			if(retimgp->width - imagep->x < imagep->width){
				cpbytes = retimgp->width - imagep->x;
			}else{
				cpbytes = imagep->width;
			}
		}else{
			ret_loc = ret_loc - imagep->x * 4; /* -x > 0 */
			im_loc = im_loc - imagep->x * 4;
			cpbytes = imagep->width + imagep->x;
		}
		//void *memcpy(void *dest, const void *src, size_t n);
		if(cpbytes > 0){
			memcpy(p + ret_loc, q + im_loc, cpbytes * 4); 
		}
	}


	return 0;
	
}

