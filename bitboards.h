/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&                !"$%!%\!%
  ^+\$%^!&   bitboards.h    &$&?`\>+
  ^\?&%+\!$                $%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef BITBOARDS_H
#define BITBOARDS_H

//system headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define 64-bit datatype
//used for bitboards and hash
#define U64 unsigned long long

//bitboard squares (Little-Endian-Rank-File)
enum squares {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};


/*********************\
*  USEFUL BITBOARDS  *
\*********************/

//files
extern U64 file_a;
extern U64 b_file;
extern U64 g_file;
extern U64 h_file;

//ranks
extern U64 rank_1;
extern U64 rank_2;
extern U64 rank_3;
extern U64 rank_4;
extern U64 rank_5;
extern U64 rank_6;
extern U64 rank_7;
extern U64 rank_8;

//castles
extern U64 wk_castle_open;
extern U64 wq_castle_open;
extern U64 bk_castle_open;
extern U64 bq_castle_open;

//others
extern U64 full;
extern U64 border;
extern U64 dark;


//not file functions
U64 not_a();
U64 not_h();
U64 not_ab();
U64 not_gh();


/***********************************************\
 * ===========================================*
                Bit Manipulations 
 * =========================================== *
\***********************************************/

//bit macros
#define set_bit(bb, sq) (bb |= (1ULL << sq))
#define get_bit(bb, sq) (bb & (1ULL << sq))
#define pop_bit(bb, sq) (get_bit(bb, sq) ? bb ^= (1ULL << sq) : 0)

U64 get_bb(U64 color, U64 piece);
U64 get_occupied(U64 white, U64 black);

int pop_count(U64 bb);
int bit_scan_forward(U64 bb);
int bit_scan_reverse(U64 bb);
int get_opposite(int color);

void printbb(U64 bb);

#endif