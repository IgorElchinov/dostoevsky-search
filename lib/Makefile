OBJ=haffman
OBJ=unordered_set
TARGET=test_$(OBJ).out
CC=gcc
MAINDIR=./test
SRCDIR=./src
BINDIR=./bin
HEADDIR=./headers
FLAGS= -Wall -iquote $(HEADDIR)
ADD= 

all: make_dirs $(TARGET)

make_dirs:
	mkdir -p ./bin

test_vector.out: test_vector.o libvector.so                         # final compilation for vector
	$(CC) $< -L$(BINDIR) -o $@ -lvector $(ADD)

test_trie.out: test_trie.o libtrie.so libvector.so                  # final compilation for trie
	$(CC) $< -L$(BINDIR) -o $@ -lvector -ltrie $(ADD)

test_haffman.out: test_haffman.o libhaffman.so                      # final compilation for haffman
	$(CC) $< -L$(BINDIR) -o $@ -lhaffman $(ADD)

test_unordered_map.out: test_unordered_map.o libunordered_map.so    # final compilation for map
	$(CC) $< -L$(BINDIR) -o $@ -lunordered_map -llist_of_map_nodes -lhash_table -llist_str $(ADD)

test_unordered_set.out: test_unordered_set.o libunordered_set.so    # final compilation for set
	$(CC) $< -L$(BINDIR) -o $@ $(ADD) -lunordered_set -lhash_table -llist_str

test_$(OBJ).o: $(MAINDIR)/test_$(OBJ).c                             # compiling example
	$(CC) -c $< -o $@ $(FLAGS) $(ADD)

$(OBJ).so: $(HEADDIR)/$(OBJ).h                                      # make libraries dependant from their header files

libhaffman.so: $(SRCDIR)/haffman.c                                    # compiling dynamic library for haffman
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libvector.so: $(SRCDIR)/vector.c                                    # compiling dynamic library for vector
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libtrie.so: $(SRCDIR)/trie.c                                        # compiling dynamic library for trie
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector

libunordered_set.so: $(SRCDIR)/unordered_set.c libhash_table.so
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lhash_table

libhash_table.so: $(SRCDIR)/hash_table.c liblist_str.so
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -llist_str

liblist_str.so: $(SRCDIR)/list_str.c
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libunordered_map.so: $(SRCDIR)/unordered_map.c liblist_of_map_nodes.so libmap_node.so libhash_table.so
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -llist_of_map_nodes -lmap_node -lhash_table

liblist_of_map_nodes.so: $(SRCDIR)/list_of_map_nodes.c libmap_node.so
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lmap_node

libmap_node.so: $(SRCDIR)/map_node.c
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

lib: lib$(OBJ).so

run: all                                                            # target to run code
	./runner $(TARGET)

clean:                                                              # target to clean binary files
	rm -f *.out *.o $(BINDIR)/*.so