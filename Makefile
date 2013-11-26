SOURCES = $(wildcard *.c) $(wildcard */*.c)

all: clean
	@gcc-4.8 $(SOURCES) -o main -O3 -Wall -fopenmp
	@time ./main res/easy.pgm
	@for i in res/blobs/*.pgm; do convert $$i -depth 16 $$i.png; done; rm res/blobs/*.pgm

clean:
	@#rm res/blobs/
