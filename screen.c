/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 07 15:21:53 CST			*
 ********************************************************
 *		after studying C 51 days		*
 *		after studying APUE 16 days		*
 ********************************************************/
/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 27 23:11:13 CST			*
 * @debug						*
 *     change all types of variable about location to   *
 *   `long' form `unsigned long', otherwise error will  *
 *   come, such as `(100 * -4) = 4294966896'.		*
 *     I find this bug in `fb_screen_add_image()', and	*
 *   change type of `screen.pixelbits' to `long' too.	*
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
	int i;

	screenp->width = fbp->fb_vinfo.xres;
	screenp->height = fbp->fb_vinfo.yres;
	screenp->screensize = fbp->fb_screensize;
	screenp->fb_start = fbp->fb_start;
	screenp->pixelbits = fbp->fb_vinfo.bits_per_pixel;
	screenp->screenstart = malloc(sizeof(char) * screenp->screensize);

	for(i = 0; i < 2; i++){
		//int fb_image_init(FB_IMAGE *imagep, int width, int height, int components);
		fb_image_init(&screenp->screen_buf[i], screenp->width,
			      screenp->height, screenp->pixelbits / 8);

		fb_image_setpos(&screenp->screen_buf[i], 0, 0);
	}

	return 0;
}

/*
 * Destory screen
 */
int fb_screen_destory(FB_SCREEN *screenp)
{
	int i;

	for(i = 0; i < 2; i++){
		fb_image_destory(&screenp->screen_buf[i]);
	}

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
	memset(screenp->screenstart, 0x00, screenp->screensize);

	return 0;
}

/* 
 * add image to screen at (x, y)
 * assume that image always is rectangle
 *
 * note:
 *   Before call this function, you SHOULD call fb_screen_optimize_image
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
 * add image to screen 
 * assume that image always is rectangle
 *
 * note:
 *   Before call this function, you SHOULD call fb_screen_optimize_image
 */
int fb_screen_add_image_fullscr(FB_SCREEN *screenp, FB_IMAGE *imagep)
{
	float px, py, pz;
	int xpos, ypos;

	xpos = (screenp->width - imagep->width) >> 1;
	ypos = (screenp->height - imagep->height) >> 1;


	px = screenp->width / (float)imagep->width;
	py = screenp->height / (float)imagep->height;

	pz = px < py ? px : py;

	//int fb_image_setpos(FB_IMAGE *imagep, int x, int y);
	fb_image_setpos(imagep, xpos, ypos);		


	if(pz < 1){
		//int fb_screen_add_image_enlarge(FB_SCREEN *screenp, FB_IMAGE *imagep,
		//             			float proportionx, float proportiony) 

		fb_screen_add_image_enlarge(screenp, imagep, pz, pz);
	}else{
		
		fb_screen_add_image(screenp, imagep);
	}

		return 0;
}

/* 
 * add image to screen at (x, y)
 * assume that image always is rectangle
 *
 * note:
 *   Before call this function, you SHOULD call fb_screen_optimize_image
 */
int fb_screen_add_image_fullscrbuf(FB_IMAGE *scrbuf, FB_IMAGE *imagep)
{
	float px, py, pz;

	px = scrbuf->width / (float)imagep->width;
	py = scrbuf->height / (float)imagep->height;

	pz = px < py ? px : py;

	if(pz < 1){
		//int fb_image_toimage(FB_IMAGE *imagep, FB_IMAGE *retimgp)
		fb_image_full_image(imagep, scrbuf, IMAGE_FULL_LOCK);
	}else{
		//int fb_image_add_image(FB_IMAGE *imagep, FB_IMAGE *retimgp);
		fb_image_add_image(imagep, scrbuf);
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

	tmpimg.x = (screenp->width - tmpimg.width) / 2;
	tmpimg.y = (screenp->height - tmpimg.height) / 2;

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

/*
 * Set image at center of screen
 */
int fb_screen_set_image_center(FB_SCREEN *screenp, FB_IMAGE *imagep)
{
	int xpos, ypos;

	xpos = (screenp->width - imagep->width) / 2;
	ypos = (screenp->height - imagep->height) / 2;
	
	fb_image_setpos(imagep, xpos, ypos);
	
	return 0;
}

/*
 * Upturn screen buf num
 */
int fb_screen_upturn_buf(FB_SCREEN *screenp, int num)
{
	memcpy(screenp->fb_start, screenp->screen_buf[num].imagestart,
	       screenp->screensize);

	return 0;
}

/*
 * clear a screen buffer
 */
int fb_screen_clear_buf(FB_SCREEN *screenp, int num)
{
	memset(screenp->screen_buf[num].imagestart, 0x0,
	       screenp->screen_buf[num].imagesize);

	return 0;
}

/*
 * Add image to framebuffer by line
 */
int fb_screen_add_image_byline(FB_IMAGE *imagep, FB_SCREEN *screenp, int line)
{
	int j;
	unsigned long ret_loc;	/* screen location */
	unsigned long im_loc;	/* image location */
	long cpbytes;
	unsigned char *p, *q;

	p = screenp->fb_start;
	q = imagep->imagestart;

	j = line - 1;
	
	/* vertical overflow check */
	if(0 < imagep->y + j && imagep->y + j < screenp->height){
		ret_loc = ((imagep->y + j) * screenp->width + imagep->x) * imagep->components;
		im_loc = j * imagep->width * imagep->components;

		/* horizontal overflow check */
		if(imagep->x >= 0){
			if(screenp->width - imagep->x < imagep->width){
				cpbytes = screenp->width - imagep->x;
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

/*
 * Add image to framebuffer by line
 */
int fb_screen_add_image_bylinev(FB_IMAGE *imagep, FB_SCREEN *screenp, int line)
{
	int j;
	unsigned long ret_loc;	/* screen location */
	unsigned long im_loc;	/* image location */
	unsigned char *p, *q;

	p = screenp->fb_start;
	q = imagep->imagestart;

	for(j = 0; j < screenp->height; j++){
		/* vertical overflow check */
		if(0 < imagep->y + j && imagep->y + j < screenp->height){
			ret_loc = ((imagep->y + j) * screenp->width + line) * imagep->components;
			im_loc = (j * imagep->width + line) * imagep->components;

			/* horizontal overflow check */
			if(line >= 0 && screenp->width - line < imagep->width){
				memcpy(p + ret_loc, q + im_loc, 4); 
			}
		}
	}

	return 0;
}

/*
 * change screen with trans
 */
int fb_screen_change_trans(FB_SCREEN *screenp, unsigned char trans)
{
	int i, j;
	unsigned char *p;

	p = screenp->screenstart;

	for(i = 0; i < screenp->height; i++){
		for(j = 0; j < screenp->width; j++){
			if(*p > trans){
				*p -= trans;
			}else{
				*p = 0;
			}
			p++;

			if(*p > trans){
				*p -= trans;
			}else{
				*p = 0;
			}
			p++;
			
			if(*p > trans){
				*p -= trans;
			}else{
				*p = 0;
			}
			p++;

			*p = trans;
			p++;
		}
	}

	return 0;
}

/*
 * set screen with trans
 */
int fb_screen_set_trans(FB_SCREEN *screenp, unsigned char trans)
{
	int i, j;
	unsigned char *p;

	p = screenp->screenstart;

	for(i = 0; i < screenp->height; i++){
		for(j = 0; j < screenp->width; j++){
			p[3] = trans;
			p += 4;
		}
	}

	return 0;
}

/*
 * update screen with trans
 */
int fb_screen_update_trans(FB_SCREEN *screenp)
{
	int i, j;
	unsigned char *p;
	unsigned char *q;
	unsigned char trans;

	p = screenp->screenstart;
	q = screenp->screen_buf[0].imagestart;

	for(i = 0; i < screenp->height; i++){
		for(j = 0; j < screenp->width; j++){
			trans = p[3];
			if(*p > trans){
				*q = *p - trans;
			}else{
				*q = 0;
			}
			p++;
			q++;

			if(*p > trans){
				*q = *p - trans;
			}else{
				*q = 0;
			}
			p++;
			q++;
			
			if(*p > trans){
				*q = *p - trans;
			}else{
				*q = 0;
			}
			p++;
			q++;

			p++;
			q++;

		}
	}

	fb_screen_upturn_buf(screenp, 0);
	return 0;
}
