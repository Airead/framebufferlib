CC	= gcc

CFLAGS	= -Wall -g -DDEBUG -lm -ljpeg -lz -lfreetype

OBJECT = testtext.c line.c pixel.c framebuffer.c plane.c circle.c \
	jpeg.c image.c screen.c font.c text.c

LDFLAGS	= 

test.out: $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

.PHONY: all clean

clean:
	rm -f *.o *~ a.out test.out
