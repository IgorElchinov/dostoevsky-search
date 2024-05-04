
OBJ=trie
TARGET=test_$(OBJ).out
CC=gcc
MAINDIR=./test
SRCDIR=./lib/src
BINDIR=./lib/bin
HEADDIR=./lib/headers
FLAGS= -g -Wall -iquote $(HEADDIR)
ADD= 

all: make_dirs lib index.out # search.out

make_dirs:
	mkdir -p lib/bin

index.out:
	gcc search/index.c -o index.out -L./lib/bin -lunordered_map -llist_of_map_nodes -lhash_table -llist_str -ltrie -lvector -lunordered_set -lhaffman $(FLAGS)

search.out:
	gcc search/search.c -o search.out

lib%.so: $(HEADDIR)/%.h                                                              # make libraries dependant from their header files

libvector.so: $(SRCDIR)/vector.c                                                     # compiling dynamic library for vector
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector

libtrie.so: $(SRCDIR)/trie.c                                                         # compiling dynamic library for trie
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector -ltrie

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

libhaffman.so: $(SRCDIR)/haffman.c                                    # compiling dynamic library for haffman
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

lib: libvector.so libtrie.so liblist_str.so libhash_table.so libunordered_set.so libmap_node.so liblist_of_map_nodes.so libunordered_map.so libhaffman.so

run: all                                                                             # target to run code
	./runner $(TARGET)

clean:                                                                               # target to clean binary files
	rm -f *.out *.o $(BINDIR)/*.so