/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&                !"$%!%\!%
  ^+\$%^!&    square.c      &$&?`\>+
  ^\?&%+\!$                $%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef SQUARE_H
#define SQUARE_H

#include "bitboards.h"
#include "position.h"
#include "attack.h"



int square_index(int color, int piece);
int square_color(int index);
int square_piece(int index);

int get_square_color(position position, int sq);
int get_square_piece(position position, int sq);

//super slow
int get_color_sq(position position, int sq);
int get_piece_sq(position position, int sq);


void printsquareboard(position position);
void bb_to_square(position *position);


void remove_square(position *position, int sq);
void add_square(position *position, int sq, int color, int piece);
void move_square(position *position, int source, int target);
void promotion_square(position *position, int source, int target, int piece);

int sq_attacked_by_pawn(position position, int sq);
int sq_attacked_by_horse(position position, int sq);
int sq_attacked_by_king(position position, int sq);
int sq_attacked_by_bishop(position position, int sq);
int sq_attacked_by_queen(position position, int sq);
int sq_attacked_by_rook(position position, int sq);

int sq_attacked(position position, int sq);

#endif