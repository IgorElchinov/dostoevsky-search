
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

libunordered_set.so: $(SRCDIR)/unordered_set.c 
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lhash_table

libhash_table.so: $(SRCDIR)/hash_table.c
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -llist_str

liblist_str.so: $(SRCDIR)/list_str.c
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libunordered_map.so: $(SRCDIR)/unordered_map.c 
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -llist_of_map_nodes -lmap_node

libunordered_map.so: $(SRCDIR)/list_of_map_nodes.c 
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lmap_node

lib: lib$(OBJ).so

run: all                                                            # target to run code
	./runner $(TARGET)

clean:                                                              # target to clean binary files
	rm -f *.out *.o