/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 21:01:04 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include "framebuffer.h"

/*
 * Open frame buffer device and
 * fill struct frambuffer information
 */
int fb_open(char *fbname, struct framebuffer *fbp)
{
	/* open frame buffer device */
	//int open(const char *pathname, int flags);
	fbp->fb_fd = open(fbname, O_RDWR);
	if(fbp->fb_fd == -1){
		fprintf(stderr, "open %s failed: %s\n", fbname, strerror(errno));
		return -1;
	}

	/* get screen information */
	//int ioctl(int d, int request, ...);
	if(ioctl(fbp->fb_fd, FBIOGET_FSCREENINFO, &fbp->fb_finfo) < 0){
		fprintf(stderr, "get fixed screen information failed: %s\n",
			strerror(errno));
		return -1;			
	}
	if(ioctl(fbp->fb_fd, FBIOGET_VSCREENINFO, &fbp->fb_vinfo) < 0){
		fprintf(stderr, "get var screen information failed: %s\n",
			strerror(errno));
		return -1;			
	}
	
	/* get screen size according to vinfo */
	fbp->fb_screensize = fbp->fb_vinfo.xres * fbp->fb_vinfo.yres
		* fbp->fb_vinfo.bits_per_pixel / 8;
	
	/* get frame buffer start pointer via mmap */
	//void *mmap(void *addr, size_t length, int prot, int flags,
        //          int fd, off_t offset);
	fbp->fb_start = mmap(NULL, fbp->fb_screensize, PROT_READ 
				| PROT_WRITE, MAP_SHARED, fbp->fb_fd, 0);
	if((void *)fbp->fb_start == MAP_FAILED){
		fprintf(stderr, "mmap failed: %s\n", strerror(errno));
		return -1;
	}

#if DEBUG
	fprintf(stdout, "fb: %dx%d/%dbits, screen: %lu btyes, "
		"mem start: %#08x\n", fbp->fb_vinfo.xres, fbp->fb_vinfo.yres,
		fbp->fb_vinfo.bits_per_pixel, fbp->fb_screensize, 
		(unsigned int)fbp->fb_start);
#endif

	return 0;
}

/*
 * Close file descriptor and munmap
 */
int fb_close(FB *fbp)
{
	//int munmap(void *addr, size_t length);
	if(munmap(fbp->fb_start, fbp->fb_screensize) < 0){
		fprintf(stderr, "munmap failed: %s\n", strerror(errno));
		return -1;
	}

	if(close(fbp->fb_fd) < 0){
		fprintf(stderr, "close file failed: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}
