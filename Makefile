CC=clang
CFLAGS= -Wall -Werror -Wpedantic -Wextra
LFLAGS=-lm
all: encode decode
encode: encode.o
	$(CC) -o encode encode.o stack.o code.o node.o io.o pq.o huffman.o $(LFLAGS)
encode.o: encode.c defines.h stack.h stack.c code.h code.c io.h io.c pq.h pq.c node.h node.c huffman.h huffman.c
	$(CC) $(CFLAGS) -c encode.c stack.c stack.h code.c code.h io.c io.h pq.c pq.h node.c node.h huffman.c huffman.h 
decode: decode.o
	$(CC) -o decode  decode.o stack.o code.o node.o io.o pq.o huffman.o $(LFLAGS)
decode.o: decode.c defines.h stack.h stack.c code.h code.c io.h io.c pq.h pq.c node.h node.c huffman.h huffman.c
	 $(CC) $(CFLAGS) -c decode.c stack.c stack.h code.c code.h io.c io.h pq.c pq.h node.c node.h huffman.c huffman.h
clean:
	rm -f encode.o stack.o code.o node.o iq.o pq.o huffman.o decode.o encode decode io.o code.h.gch io.h.gch pq.h.gch node.h.gch huffman.h.gch stack.h.gch
format:
	clang-format -i -style=file *.[ch]
