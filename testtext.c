/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 24 12:39:37 CST			*
 ********************************************************
 *		after studying C 68 days		*
 *		after studying APUE 33 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "font.h"
#include "framebuffer.h"
#include "pixel.h"

int my_draw_bitmap(FB *fbp, FT_Bitmap*  bitmap, FT_Int x, FT_Int y)
{
	COLOR_32 color, backcolor;
	FB_POINT point;

	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;
	unsigned char ch;

	color = fb_formatRGB(255, 255, 255);
	backcolor = fb_formatRGB(0, 0, 0);
	
	for ( j = y, q = 0; j < y_max; j++, q++ )
	{

		for ( i = x, p = 0; i < x_max; i++, p++ ){
			ch = bitmap->buffer[q * bitmap->width + p];
			if(ch > 128){
				//int fb_set_pixel(FB_POINT *point, int x, int y, COLOR_32 color);
				fb_set_pixel(&point, i, j, color);
				//int fb_draw_pixel(struct framebuffer *fbp, FB_POINT *point);
				fb_draw_pixel(fbp, &point);
			}else{
				fb_set_pixel(&point, i, j, backcolor);
				fb_draw_pixel(fbp, &point);
			}
			//	image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
	
	return 0;
}


int main(int argc, char *argv[])
{
	FB fb;

	FB_FONT font;
	FT_GlyphSlot slot;	/* a small shortcut */
	int pen_x, pen_y, n;
	FT_Error error;
	
	char *text = "Airead, 范人豪";
	int num_chars;
	int fontsize;

	fontsize = 96;

	fprintf(stdout, "%s\n", text);

	//int fb_open(char *fbname, FB *fbp);
	fb_open("/dev/fb0", &fb);

	//int fb_font_open(char *fontname, FB_FONT *ffp);
	fb_font_open("Times.ttf", &font);

	fprintf(stdout, "fontsize = %d\n", fontsize);

	//int fb_font_set_charsize(FB_FONT *ffp, int fontsize);
	fb_font_set_charsize(&font, fontsize);

	//int fb_text_show(FB_SCREEN *screenp, char *str, FB_FONT *ffp, int x, int y);
	

	slot = font.face->glyph;
	pen_x = 0;
	pen_y = 300;
		
	num_chars = strlen(text);
		
	for(n = 0; n < num_chars; n++){
		FT_UInt glyph_index;
			
		/* retrieve glyph index from character code*/
		glyph_index = FT_Get_Char_Index(font.face, text[n]);
			
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(font.face, glyph_index, FT_LOAD_DEFAULT);
		if(error){
			fprintf(stderr, "FT_Load_Glyph() failed: %d", error);
		}
			
		/* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(font.face->glyph, FT_RENDER_MODE_NORMAL);
		if(error){
			fprintf(stderr, "FT_Render_Glyph() failed: %d", error);
		}
			
		/* now, draw to our target surface */
		my_draw_bitmap(&fb, &slot->bitmap, 
			       pen_x + slot->bitmap_left,
			       pen_y - slot->bitmap_top);

		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
	}

	//int fb_close(FB *fbp);
	fb_close(&fb);

	//int fb_font_close(FB_FONT *ffp);
	fb_font_close(&font);

	return 0;
}
