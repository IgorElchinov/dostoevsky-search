
OBJ=trie
TARGET=test_$(OBJ).out
CC=gcc
MAINDIR=./test
SRCDIR=./lib/src
BINDIR=./lib/bin
HEADDIR=./lib/headers
FLAGS= -g -Wall -iquote $(HEADDIR)
ADD= 

all: lib index.out search.out

index.out:
	gcc search/index.c -o index.out

search.out:
	gcc search/search.c -o search.out

libvector.so: $(HEADDIR)/vector.h                                   # make libraries dependant from their header files
libvector.so: $(HEADDIR)/vector.h                                   # make libraries dependant from their header files

libvector.so: $(SRCDIR)/vector.c                                    # compiling dynamic library for vector
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libtrie.so: $(SRCDIR)/trie.c                                        # compiling dynamic library for trie
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector

lib: libvector.so libtrie.so

run: all                                                            # target to run code
	./runner $(TARGET)

clean:                                                              # target to clean binary files
	rm -f *.out *.o