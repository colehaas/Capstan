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
#include "generate.h"
#include "square.h"

//for make function
int get_rank(int sq);

/***************************\
       MAKE & UNMAKE   
\***************************/

position make(position from, int move);
U64 perft(position start, int depth);
void play(position position);

#endif