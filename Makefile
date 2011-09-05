CC	= gcc
CFLAGS	= -Wall -g -DDEBUG -lm
OBJECT = test.c line.c pixel.c framebuffer.c plane.c circle.c
LDFLAGS	= 

test.out: $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

.PHONY: all clean

clean:
	rm -f *.o *~ a.out test.out
