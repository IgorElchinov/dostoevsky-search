
OBJ=trie
TARGET=test_$(OBJ).out
CC=gcc
MAINDIR=./test
SRCDIR=./src
BINDIR=./bin
HEADDIR=./headers
FLAGS= -g -Wall -iquote $(HEADDIR)
ADD= 

all: $(TARGET)

test_vector.out: test_vector.o libvector.so                         # final compilation for vector
	$(CC) $< -L$(BINDIR) -o $@ -lvector $(ADD)

test_trie.out: test_trie.o libtrie.so libvector.so                  # final compilation for trie
	$(CC) $< -L$(BINDIR) -o $@ -lvector -ltrie $(ADD)

test_$(OBJ).o: $(MAINDIR)/test_$(OBJ).c                             # compiling example
	$(CC) -c $< -o $@ $(FLAGS) $(ADD)

$(OBJ).so: $(HEADDIR)/$(OBJ).h                                      # make libraries dependant from their header files

libvector.so: $(SRCDIR)/vector.c                                    # compiling dynamic library for vector
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libtrie.so: $(SRCDIR)/trie.c                                        # compiling dynamic library for trie
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector

lib: lib$(OBJ).so

run: all #target to run code
	./runner $(TARGET)

clean:
	rm -f *.out *.o