/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 20:55:13 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#ifndef FB_FRAMEBUFFER
#define FB_FRAMEBUFFER

#include <linux/fb.h>

struct framebuffer{
	int fb_fd;		/* file descriptor */
	struct fb_fix_screeninfo fb_finfo;
	struct fb_var_screeninfo fb_vinfo;
	unsigned long fb_screensize; /* bytes */
	unsigned char *fb_starting;	/* pointer mmap framebuffer starting */
};

unsigned char *fb_open(char *fbname, struct framebuffer *fbp);
int fb_close(struct framebuffer *fbp);

#endif
