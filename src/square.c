/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&                !"$%!%\!%
  ^+\$%^!&    square.c      &$&?`\>+
  ^\?&%+\!$                $%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#include "square.h"

/*
 * color, piece, index
 * index = ((color * 8) + piece)
 * color = (index / 8)
 * piece = (index % 8)
*/

int square_index(int color, int piece) {
  return ((color * 8) + piece);
}

int square_color(int index) {
  return (index / 8);
}

int square_piece(int index) {
  return (index % 8);
}


int get_square_color(int square[64], int sq) {
  return square_color(square[sq]);
}

int get_square_piece(int square[64], int sq) {
  return square_piece(square[sq]);
}


//super slow
int get_color_sq(position position, int sq) {
  U64 bb_sq = 0x0ULL;
  set_bit(bb_sq, sq);
  if (bb_sq & position.boards[white]) return white;
  if (bb_sq & position.boards[black]) return black;
  return -1;
}

int get_piece_sq(position position, int sq) {
  U64 bb_sq = 0x0ULL;
  set_bit(bb_sq, sq);
  if (bb_sq & position.boards[pawn]) return pawn;
  if (bb_sq & position.boards[horse]) return horse;
  if (bb_sq & position.boards[king]) return king;
  if (bb_sq & position.boards[queen]) return queen;
  if (bb_sq & position.boards[rook]) return rook;
  if (bb_sq & position.boards[bishop]) return bishop;
  return -1;
}

//print bitboard
void printsquareboard(int square[64]) {
  
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      int sq = (rank * 8) + file;
      if (!file) printf("%d   ", rank + 1);
      printf("%X  ", square[sq]);
    }
    printf("\n");
  }
  printf("\n    a  b  c  d  e  f  g  h\n\n");
}

void bb_to_square(position *position) {
    
  int color = 0;
  int piece = 0;
  
  for (int sq = 0; sq < 64; sq++) {
    color = get_color_sq(*position, sq);
    piece = get_piece_sq(*position, sq);
    if (color != -1) {
      position->square[sq] = square_index(color, piece);
    }
    else {
      position->square[sq] = 0;
    }
    //printf("\n%5d:%5d%5d%5X", sq, color, piece, position->square[sq]);

  }
}

void remove_square(int square[64], int sq) {
  square[sq] = 0;
}

void add_square(int square[64], int sq, int color, int piece) {
  square[sq] = square_index(color, piece);
}

void move_square(int square[64], int source, int target) {
  square[target] = square[source];
  square[source] = 0;
}

void promotion_square(int square[64], int source, int target, int piece) {
  int color = square_color(square[source]);
  int index = square_index(color, piece);
  square[target] = index;
  square[source] = 0;
}

int sq_attacked_by_pawn(int square[64], int sq) {
  //int color = position.turn;
  int color = get_square_color(square, sq);
  //index of attacking piece
  int attacker = 0;
  int attack_sq = 0;
  int count = 0;

  U64 attack = 0x0ULL;
  //white
  if (color == white) {
    //index for black pawn
    attacker = square_index(black, pawn);
    //bitboard with possible attack positions
    attack = pawn_attacks[white][sq];
  }
  //black
  else if (color == black) {
    //index for white pawn
    attacker = square_index(white, pawn);
    //bitboard with possible attack positions
    attack = pawn_attacks[black][sq];
  }
  
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_horse(int square[64], int sq) {
  //int color = position.turn;
  int color = get_square_color(square, sq);
  //index of opposite colored horse
  int attacker = square_index(get_opposite(color), horse);
  int attack_sq = 0;
  int count = 0;

  U64 attack = horse_attacks[sq];
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_king(int square[64], int sq) {
  //int color = position.turn;
  int color = get_square_color(square, sq);
  //index of opposite colored king
  int attacker = square_index(get_opposite(color), king);
  int attack_sq = 0;
  int count = 0;

  U64 attack = king_attacks[sq];
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_bishop(int square[64], int sq, U64 occupied) {
  //int color = position.turn;
  int color = get_square_color(square, sq);
  //index of opposite colored bishop
  int attacker = square_index(get_opposite(color), bishop);
  int attack_sq = 0;
  int count = 0;
  U64 attack = get_bishop_attack(occupied, sq);

  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_rook(int square[64], int sq, U64 occupied) {
  //int color = position.turn;
  int color = get_square_color(square, sq);
  //index of opposite colored rook
  int attacker = square_index(get_opposite(color), rook);
  int attack_sq = 0;
  int count = 0;
  U64 attack = get_rook_attack(occupied, sq);
  
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_queen(int square[64], int sq, U64 occupied) {
  //int color = position.turn;
  int color = get_square_color(square, sq);
  //index of opposite colored horse
  int attacker = square_index(get_opposite(color), queen);
  int attack_sq = 0;
  int count = 0;
  U64 attack = get_queen_attack(occupied, sq);
  
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}



//returns # if attacked, 0 if not
int sq_attacked(int square[64], int sq, U64 occupied) {
  int count = 0;
  count += sq_attacked_by_pawn(square, sq);
  count += sq_attacked_by_horse(square, sq);
  count += sq_attacked_by_king(square, sq);
  count += sq_attacked_by_queen(square, sq, occupied);
  count += sq_attacked_by_rook(square, sq, occupied);
  count += sq_attacked_by_bishop(square, sq, occupied);
  return count;
}
