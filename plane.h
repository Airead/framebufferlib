/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 04 21:38:52 CST			*
 ********************************************************
 *		after studying C 48 days		*
 *		after studying APUE 13 days		*
 ********************************************************/

#ifndef FB_PLANE
#define FB_PLANE

#define FB_ROTATION_FIXPOINT 0x01
#define FB_ROTATION_ORIGIN 0x00
#define FB_ROTATION_CENTER 0x02

typedef struct fb_rect{
	FB_POINT left_top;
	int width;
	int height;
}FB_RECT;

int fb_draw_background(FB *fbp, COLOR_32 color);
int fb_draw_rect(FB *fbp, FB_RECT *fb_rect);
int fb_set_rect(FB_RECT *fb_rectp, FB_POINT *point, int width, int height);
int fb_rect_rotation(FB *fbp, FB_RECT *fb_rectp, double radian, int flag, FB_POINT *fixpoint);
int fb_rect_tranform(FB *fbp, FB_RECT *fb_rectp, int flag, float hs, float vs, FB_POINT *fixpoint);

#endif
