/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 20:55:13 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#ifndef FB_FRAMEBUFFER_H
#define FB_FRAMEBUFFER_H

#include <linux/fb.h>

typedef struct framebuffer{
	int fb_fd;		/* file descriptor */
	struct fb_fix_screeninfo fb_finfo;
	struct fb_var_screeninfo fb_vinfo;
	unsigned long fb_screensize; /* bytes */
	unsigned char *fb_start;	/* pointer mmap framebuffer starting */
}FB;

int fb_open(char *fbname, FB *fbp);
int fb_close(FB *fbp);
int fb_clear(FB *fbp);
int fb_clear_startpoint(unsigned char *start, unsigned long size);

#endif
