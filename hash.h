#ifndef HASH_H
#define HASH_H

#include "bitboards.h"
#include "square.h"

typedef struct tpt {
    U64 hash_key;
    int best_move;
    int depth;
    int score;
    int nodeType;
    int age;
} tpt;

extern U64 hash_values[13][64];
extern tpt transp[0x400000];

U64 xorshift(U64);
void init_hash();


#endif
