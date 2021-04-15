/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&                !"$%!%\!%
  ^+\$%^!&   bitboards.c    &$&?`\>+
  ^\?&%+\!$                $%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#include "bitboards.h"

U64 file_a = 0x0101010101010101ULL;
U64 b_file = 0x0202020202020202ULL;
U64 g_file = 0x4040404040404040ULL;
U64 h_file = 0x8080808080808080ULL;

//ranks
U64 rank_1 = 0x00000000000000ffULL;
U64 rank_2 = 0x000000000000ff00ULL;
U64 rank_3 = 0x0000000000ff0000ULL;
U64 rank_4 = 0x00000000ff000000ULL;
U64 rank_5 = 0x000000ff00000000ULL;
U64 rank_6 = 0x0000ff0000000000ULL;
U64 rank_7 = 0x00ff000000000000ULL;
U64 rank_8 = 0xff00000000000000ULL;

//castles
U64 wk_castle_open = 0x0000000000000060ULL;
U64 wq_castle_open = 0x000000000000000EULL;
U64 bk_castle_open = 0x6000000000000000ULL;
U64 bq_castle_open = 0x0E00000000000000ULL;

//others
U64 full = 0xffffffffffffffffULL;
U64 border = 0xff818181818181ffULL;
U64 dark = 0xAA55AA55AA55AA55ULL;


//not file functions
U64 not_a() {return ~file_a;}
U64 not_h() {return ~h_file;}

U64 not_ab() {return ~(file_a | b_file);}
U64 not_gh() {return ~(g_file | h_file);}

/***********************************************\
 * ===========================================*
                Bit Manipulations
 * =========================================== *
\***********************************************/

//bit macros
#define set_bit(bb, sq) (bb |= (1ULL << sq))
#define get_bit(bb, sq) (bb & (1ULL << sq))
#define pop_bit(bb, sq) (get_bit(bb, sq) ? bb ^= (1ULL << sq) : 0)

U64 get_bb(U64 color, U64 piece) {
  return (color & piece);
}

U64 get_occupied(U64 white, U64 black) {
  return (white | black);
}

int pop_count(U64 bb) {
  int count = 0;
  while (bb) {
    count++;
    bb &= bb - 1;
  }
  return count;
}

int bit_scan_forward(U64 bb) {
  if (bb) return pop_count((bb & -bb) - 1);
  else return -1;
}

int bit_scan_reverse(U64 bb) {
  
  //get bb with all 1's <= msb 
  bb |= bb >> 1;
  bb |= bb >> 2;
  bb |= bb >> 4;
  bb |= bb >> 8;
  bb |= bb >> 16;
  bb |= bb >> 32;

  //next step will cause a bb to = 0, msb = -1
  if (bb == full) return h8;

  //get bb with the only 1 in the spot 1 greater than msb
  bb+=1;
  //get bb with the only 1 in msb
  bb = bb >> 1;

  return bit_scan_forward(bb);

}

int get_opposite(int color) {
    //return 1 - color;
    return color ^ 1;
}


//print bitboard
void printbb(U64 bb) {
  int r, f;

  for (r = 7; r >= 0; r--) {
    for (f = 0; f < 8; f++) {
      int sq = r*8+f;
      if (!f) printf("%d   ", r+1);
      printf("%d  ", get_bit(bb, sq) ? 1 : 0);
    }
    printf("\n");
  }
  printf("\n    a  b  c  d  e  f  g  h\n\n");
  printf("    bitboard: %llX\n\n", bb);

  }
