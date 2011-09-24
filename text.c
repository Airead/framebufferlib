/********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 24 12:38:26 CST			*
 ********************************************************
 *		after studying C 68 days		*
 *		after studying APUE 33 days		*
 ********************************************************/

#include "font.h"
#include "screen.h"

/*
 * Show a string at (x, y) in screen
 */
int fb_text_show(FB_SCREEN *screenp, char *str, FB_FONT *ffp, int x, int y)
{
	FT_GlyphSlot slot;
	FT_Error error;
	int n;
	int num_chars;

	slot = ffp->face->glyph;

	num_chars = strlen(str);

	for(n = 0; n < num_chars; n++){
		FT_UInt glyph_index;
			
		/* retrieve glyph index from character code*/
		glyph_index = FT_Get_Char_Index(ffp->face, str[n]);
			
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(ffp->face, glyph_index, FT_LOAD_DEFAULT);
		if(error){
			fprintf(stderr, "FT_Load_Glyph() failed: %d", error);
		}
			
		/* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(ffp->face->glyph, FT_RENDER_MODE_NORMAL);
		if(error){
			fprintf(stderr, "FT_Render_Glyph() failed: %d", error);
		}
			
		/* now, draw to our target surface */
		fb_font_draw_bitmap(fb, &slot->bitmap, 
			       pen_x + slot->bitmap_left,
			       pen_y - slot->bitmap_top);

		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
	}

	
	return 0;
}
