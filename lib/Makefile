

TARGET=trie
CC=gcc
MAINDIR=./test
SRCDIR=./src
BINDIR=./bin
HEADDIR=./headers
FLAGS= -g -Wall -iquote $(HEADDIR)
ADD= 
DEPENDENCIES= 

all: test_$(TARGET).out

test_$(TARGET).out: test_$(TARGET).o lib$(TARGET).so $(TARGET)_dependencies #final compilation
	$(CC) $< -L$(BINDIR) -o $@ -l$(TARGET) $(DEPENDENCIES) $(ADD)

test_$(TARGET).o: $(MAINDIR)/test_$(TARGET).c #compiling example
	$(CC) -c $< -o $@ $(FLAGS) $(ADD)

$(TARGET).so: $(HEADDIR)/$(TARGET).h

lib$(TARGET).so: $(SRCDIR)/$(TARGET).c $(TARGET)_dependencies #compiling dynamic library
	$(CC) -c --shared $< -o $(BINDIR)/$@ $(FLAGS) -fPIC $(DEPENDENCIES)

lib: lib$(TARGET).so

superlib: libcontainers.so

libcontainers.so: $(HEADDIR)/containers.h

libcontainers.so: $(SRCDIR)/*.c
	$(CC) --shared $^ -o $(BINDIR)/$@ $(FLAGS) -L$(BINDIR) -fPIC

run: all #target to run code
	./runner test_$(TARGET).out

clean:
	rm -f *.out *.o

unordered_set_dependencies:
	$(eval DEPENDENCIES += -llist_str)
	$(eval DEPENDENCIES += -lhash_table)

hash_table_dependencies:
	$(eval DEPENDENCIES += -llist_str)

list_of_map_nodes_dependencies:
	$(eval DEPENDENCIES += -lmap_node)


unordered_map_dependencies:
	$(eval DEPENDENCIES += -llist_of_map_nodes)
	$(eval DEPENDENCIES += -lmap_node)
	$(eval DEPENDENCIES += -lhash_table)
	$(eval DEPENDENCIES += -llist_str)

list_str_dependencies:

vector_dependencies:

trie_dependencies:
	$(eval DEPENDENCIES += -lvector)