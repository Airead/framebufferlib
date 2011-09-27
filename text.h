/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 24 17:41:41 CST			*
 ********************************************************
 *		after studying C 68 days		*
 *		after studying APUE 33 days		*
 ********************************************************/

#ifndef FB_TEXT_H
#define FB_TEXT_H

#include "screen.h"
#include "font.h"

int fb_text_show(FB_SCREEN *screenp, char *str, FB_FONT *ffp, int x, int y);
int fb_text_show_cn(FB_SCREEN *screenp, unsigned char *str, FB_FONT *ffp, int x, int y);
unsigned long fb_text_utf8tolong(unsigned char *str, int *offset);

#endif
