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

int get_square_color(position position, int sq) {
  return square_color(position.square[sq]);
}

int get_square_piece(position position, int sq) {
  return square_piece(position.square[sq]);
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
void printsquareboard(position position) {
  
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      int sq = (rank * 8) + file;
      if (!file) printf("%d   ", rank + 1);
      printf("%X  ", position.square[sq]);
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

void remove_square(position *position, int sq) {
  position->square[sq] = 0;
}

void add_square(position *position, int sq, int color, int piece) {
  position->square[sq] = square_index(color, piece);
}

void move_square(position *position, int source, int target) {
  position->square[target] = position->square[source];
  position->square[source] = 0;
}

void promotion_square(position *position, int source, int target, int piece) {
  int color = square_color(position->square[source]);
  int index = square_index(color, piece);
  position->square[target] = index;
  position->square[source] = 0;
}



int sq_attacked_by_pawn(position position, int sq) {
  //int color = position.turn;
  int color = position.turn;
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
    if (position.square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_horse(position position, int sq) {
  //int color = position.turn;
  int color = position.turn;
  //index of opposite colored horse
  int attacker = square_index(get_opposite(color), horse);
  int attack_sq = 0;
  int count = 0;

  U64 attack = horse_attacks[sq];
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (position.square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_king(position position, int sq) {
  int color = position.turn;
  //int color = get_square_color(position, sq);
  //index of opposite colored king
  int attacker = square_index(get_opposite(color), king);
  int attack_sq = 0;
  int count = 0;

  U64 attack = king_attacks[sq];
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (position.square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_bishop(position position, int sq) {
  int color = position.turn;
  //int color = get_square_color(position, sq);
  //index of opposite colored bishop
  int attacker = square_index(get_opposite(color), bishop);
  int attack_sq = 0;
  int count = 0;
  U64 occupied = get_occupied(position.boards[white], position.boards[black]);
  U64 attack = get_bishop_attack(occupied, sq);

  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (position.square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_rook(position position, int sq) {
  int color = position.turn;
  //int color = get_square_color(position, sq);
  //index of opposite colored rook
  int attacker = square_index(get_opposite(color), rook);
  int attack_sq = 0;
  int count = 0;
  U64 occupied = get_occupied(position.boards[white], position.boards[black]);
  U64 attack = get_rook_attack(occupied, sq);
  
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (position.square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

int sq_attacked_by_queen(position position, int sq) {
  int color = position.turn;
  //int color = get_square_color(position, sq);
  //index of opposite colored queen
  int attacker = square_index(get_opposite(color), queen);
  int attack_sq = 0;
  int count = 0;
  U64 occupied = get_occupied(position.boards[white], position.boards[black]);
  U64 attack = get_queen_attack(occupied, sq);
  
  while(attack) {
    attack_sq = bit_scan_forward(attack);
    if (position.square[attack_sq] == attacker) count++;
    pop_bit(attack, attack_sq);
  }
  return count; 
}

//returns # if attacked, 0 if not
int sq_attacked(position position, int sq) {
  int count = 0;
  count += sq_attacked_by_pawn(position, sq);
  count += sq_attacked_by_horse(position, sq);
  count += sq_attacked_by_king(position, sq);
  count += sq_attacked_by_queen(position, sq);
  count += sq_attacked_by_rook(position, sq);
  count += sq_attacked_by_bishop(position, sq);
  return count;
}
