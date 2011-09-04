CC	= gcc
CFLAGS	= -Wall -g
OBJECT = line.c pixel.c framebuffer.c
LDFLAGS	= 

test: $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

.PHONY: all clean

clean:
	rm -f *.o *~ a.out $(TARGETS)
