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

int get_square_color(int square[64], int sq);
int get_square_piece(int square[64], int sq);

//super slow
int get_color_sq(position position, int sq);
int get_piece_sq(position position, int sq);


void printsquareboard(int square[64]);
void bb_to_square(position *position);


void remove_square(int square[64], int sq);
void add_square(int square[64], int sq, int color, int piece);
void move_square(int square[64], int source, int target);
void promotion_square(int square[64], int source, int target, int piece);

int sq_attacked_by_pawn(int square[64], int sq);
int sq_attacked_by_horse(int square[64], int sq);
int sq_attacked_by_king(int square[64], int sq);
int sq_attacked_by_bishop(int square[64], int sq, U64 occupied);
int sq_attacked_by_queen(int square[64], int sq, U64 occupied);
int sq_attacked_by_rook(int square[64], int sq, U64 occupied);

int sq_attacked(int square[64], int sq, U64 occupied);

#endif