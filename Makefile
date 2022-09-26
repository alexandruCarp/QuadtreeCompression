CC=gcc
DEPS=compress.h structs.h coada.h
build: quadtree
quadtree: main.o compress.o coada.o decompress.o mirror.o
	$(CC) -g -std=c11 -o quadtree main.o coada.o compress.o decompress.o mirror.o -lm
%.o: %.c $(DEPS)
	$(CC) -g -std=c11 -c -o $@ $<
clean:
	rm -f *.o quadtree *.ppm *.out