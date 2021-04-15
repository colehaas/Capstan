/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   chess.c   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef GENERATE_H
#define GENERATE_H

#include "bitboards.h"
#include "attack.h"
#include "position.h"
#include "square.h"

/*********************\
* MOVE REPRESENTATION *
\*********************/

/******* MOVE ENCODING FORMAT ********\

  c: color
  p: piece
  w: promotion
  x: capture
  y: other y
  z: other z
  s: source_square
  t: target_square
  
  cppp wxyz tttt ttss ssss:

  wxyz:
  ______________
  0000: quiet
  0001: double pawn push
  0010: queen castle
  0011: king castle
  _________________
  0100: capture
  0101: enpassant
  ______________________
  1000: horse promotion
  1001: bishop promotion
  1010: rook promotion
  1011: queen promotion
  ______________________________
  1100: horse promotion capture
  1101: bishop promotion capture
  1110: rook promotion capture
  1111: queen promotion capture 

\*************************************/

#define encode_move(source, target, promotion, capture, y, z, piece, color) \
  (source) | (target << 6) | \
  (promotion << 12) | (capture << 13) | (y << 14) | (z << 15) | \
  (piece << 16) | (color << 19)

#define decode_source(move) (move&0x3f)
#define decode_target(move) ((move&0xfc0) >> 6)
#define decode_promotion(move) ((move&0x1000) >> 12)
#define decode_capture(move) ((move&0x2000) >> 13)
#define decode_y(move) ((move&0x4000) >> 14)
#define decode_z(move) ((move&0x8000) >> 15)
#define decode_piece(move) ((move&0x70000) >> 16)
#define decode_color(move) ((move&0x80000) >> 19)


typedef struct  {
  unsigned int moves[256];
  unsigned int count;
} move_list;


//print bitboard

void printpiecepretty(int color, int piece);
void printgb(position position);
void printsq(int sq);
void printcolor(int color);
void printpiece(int piece);
void printmove(int move);
void printmoves(move_list moves);

int promotion_piece(int y, int z);

//function to get all pseudo legal moves for a color
move_list generate_moves(position position);

#endif