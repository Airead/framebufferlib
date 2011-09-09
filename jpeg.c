 /********************************************************
 * @author  Airead Fan <fgh1987168@gmail.com>		*
 * @date    2011 9月 06 23:12:38 CST			*
 ********************************************************
 *		after studying C 50 days		*
 *		after studying APUE 15 days		*
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <jpeglib.h>
#include "image.h"
#include "jpeg.h"

/*
 * load a jpeg file and fill FB_IMAGE
 */
int fb_load_jpeg(FB_IMAGE *imagep, char *filename)
{
	FILE *infile;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char *imagearr;
	JSAMPROW row_pointer[1];
	int i;
	unsigned long location = 0;
	
	if((infile = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "fopen %s failed: %s\n", filename, strerror(errno));
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	/* initial FB_IMAGE information */
	imagep->width = cinfo.image_width;
	imagep->height = cinfo.image_height;
	imagep->components = cinfo.num_components;
	imagep->imagesize = imagep->width * imagep->height * imagep->components;

	jpeg_start_decompress(&cinfo);

	/* malloc */
	imagearr = (unsigned char*)malloc(imagep->imagesize);
	row_pointer[0] = (unsigned char *)malloc(imagep->width * imagep->components);
	
	while(cinfo.output_scanline < cinfo.image_height){
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for(i = 0; i < cinfo.image_width * cinfo.num_components; i++){
			imagearr[location++] = row_pointer[0][i];
		}
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	imagep->imagestart = imagearr;
	
	//clear temp
	free(row_pointer[0]);
	fclose(infile);

	return 0;
}

