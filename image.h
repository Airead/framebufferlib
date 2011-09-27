/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 07 09:04:52 CST			*
 ********************************************************
 *		after studying C 51 days		*
 *		after studying APUE 16 days		*
 ********************************************************/

#ifndef FB_IMAGE_H
#define FB_IMAGE_H

#include "framebuffer.h"

#define IMAGE_FULL_LOCK 1
#define IMAGE_FULL_UNLOCK 0

typedef struct image{
	int x;			/* xpos */
	int y;			/* ypos */
	int width;		/* pix */
	int height;		/* pix */
	float proportionx;	/* proportion h*/
	float proportiony;	/* proportion v*/
	int components;		/* bytes */
	unsigned long imagesize;	/* bytes */
	unsigned char *imagestart;	/* image start */
}FB_IMAGE;

int fb_image_init(FB_IMAGE *imagep, int width, int height, int components);
int fb_image_show(FB *fbp, FB_IMAGE *image);
int fb_imageinfo(FB_IMAGE *imagep);
int fb_image_transform(FB *fbp, FB_IMAGE *imagep, float hs, float vs);
int fb_image_transformm(FB *fbp, FB_IMAGE *imagep, float hs, float vs);
int fb_image_transformm2(FB *fbp, FB_IMAGE *imagep, float hs, float vs);
int fb_image_destory(FB_IMAGE *imagep);
int fb_image_enlarge(FB_IMAGE *imagep, FB_IMAGE *retimgp, float proportionx, float proportiony);
int fb_image_enlarge_setcenter(FB_IMAGE *image, FB_IMAGE *retimgp);
int fb_image_setpos(FB_IMAGE *imagep, int x, int y);
int fb_image_rotate(FB_IMAGE *imagep, FB_IMAGE *retimgp, float radian);
int fb_image_rotate2(FB_IMAGE *imagep, FB_IMAGE *retimgp, float radian);
int fb_image_getmini(FB_IMAGE *imagep, FB_IMAGE *retimgp, int img_width, int img_height);
int fb_image_full_image(FB_IMAGE *imagep, FB_IMAGE *retimgp, int flag);
int fb_image_add_image(FB_IMAGE *imagep, FB_IMAGE *retimgp);
int fb_image_add_image_byline(FB_IMAGE *imagep, FB_IMAGE *retimgp, int line);

#endif
