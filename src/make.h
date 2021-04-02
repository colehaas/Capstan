/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   chess.c   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef MAKE_H
#define MAKE_H

#include "position.h"

//for make function
int get_rank(int sq);

/***************************\
       MAKE & UNMAKE   
\***************************/

void make_quiet(position *position, int source, int target, int color, int piece);
void make_capture(position *position, int source, int target, int color, int piece, int capture);
void make_quiet_promotion(position *position, int source, int target, int color, int piece, int promotion);
void make_capture_promotion(position *position, int source, int target, int color, int piece, int capture, int promotion);


void unmake(position to);
position make(position from, int move, int *illegal);
U64 perft(position start, int depth);
void play(position position);

#endif