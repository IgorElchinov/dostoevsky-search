
OBJ=trie
TARGET=test_$(OBJ).out
CC=gcc
MAINDIR=./test
SRCDIR=./lib/src
BINDIR=./lib/bin
HEADDIR=./lib/headers
FLAGS= -g -Wall -iquote $(HEADDIR)
ADD= 
DOCS=

all: make_dirs lib index.out search.out

make_dirs:
	mkdir -p lib/bin

index.out: libunordered_map.so libhash_table.so libunordered_set.so libhaffman.so libstack.so
	gcc search/index.c -o index.out -L$(BINDIR) -lunordered_map -llist_of_map_nodes -lhash_table -llist_str -ltrie -lvector -lunordered_set -lhaffman -lstack $(FLAGS)

search.out: libhaffman.so libtrie.so libvector.so libstack.so
	gcc search/search.c -o search.out -L$(BINDIR) -lhaffman -ltrie -lvector -lstack $(FLAGS)

lib%.so: $(HEADDIR)/%.h                                                              # make libraries dependant from their header files

libvector.so: $(SRCDIR)/vector.c                                                     # compiling dynamic library for vector
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector

libtrie.so: $(SRCDIR)/trie.c                                                         # compiling dynamic library for trie
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lvector -lstack -ltrie

libunordered_set.so: $(SRCDIR)/unordered_set.c libhash_table.so                      # compiling dynamic library for unordered_set
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lhash_table

libhash_table.so: $(SRCDIR)/hash_table.c liblist_str.so                              # compiling dynamic library for hash_table
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -llist_str

liblist_str.so: $(SRCDIR)/list_str.c                                                 # compiling dynamic library for list_str
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libunordered_map.so: $(SRCDIR)/unordered_map.c liblist_of_map_nodes.so libmap_node.so libhash_table.so # compiling dynamic library for unordered_map
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -llist_of_map_nodes -lmap_node -lhash_table

liblist_of_map_nodes.so: $(SRCDIR)/list_of_map_nodes.c libmap_node.so                # compiling dynamic library for list_of_map_nodes
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC -lmap_node

libmap_node.so: $(SRCDIR)/map_node.c                                                 # compiling dynamic library for map_node
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libhaffman.so: $(SRCDIR)/haffman.c                                                   # compiling dynamic library for haffman
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

libstack.so: $(SRCDIR)/stack.c                                                       # compiling dynamic library for stack
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC

lib: libvector.so libtrie.so liblist_str.so libhash_table.so libunordered_set.so libmap_node.so liblist_of_map_nodes.so libunordered_map.so libhaffman.so libstack.so

run: all                                                                             # target to run code
	./index.out $(DOCS)
	./search.out index.txt

clean:                                                                               # target to clean binary files
	rm -f *.out *.o $(BINDIR)/*.so