#ifndef PERFT_H
#define PERFT_H

#include "bitboards.h"
#include "position.h"
#include "make.h"



U64 perft_enpassant(int move);
void perft_test(char *in, int count, int depth);
U64 perft(position *pos, gamelist *gl, int depth);
void play(position *pos, gamelist *gl);


#endif