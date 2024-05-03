#include "haffman.h"


int
main() {
    compress("war_and_peace.txt", "compressed_war_and_peace2.txt");
    decompress("compressed_war_and_peace2.txt", "decompressed_war_and_peace2.txt");
    // compress("q.txt", "q_c.txt", "decompressed_war_and_peace.txt");
    // decompress("q_c.txt", "dq_c.txt");
}