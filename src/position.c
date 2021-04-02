/*************************************\
 * ================================= *
   \^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
   !%^`$!"\               %!"$%!%\!%
   +\$%^!    position.h    !^$&%?`\>
   \?&%+\!+               &$%\!%$!%$
   $%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#include "position.h"

void init_square(position *position) {
  for (int sq = 0; sq < 64; sq++) {
    position->square[sq] = 0;
  }
}

void init_enpassant(position *position) {
  position->enpassant = encode_enpassant(0, 0);
}

void init_castle(position *position) {
  position->castle = encode_castle(1, 1, 1, 1);
}

void init_turn(position *position) {
  position->turn = white;
}

void init_game_list(position *position) {
  position->game_list.count = 0;
}

void init_boards(position *position) {

  position->boards[white] = 0xffffULL;
  position->boards[black] = 0xffff000000000000ULL;
  position->boards[pawn] = 0x00ff00000000ff00ULL;

  set_bit(position->boards[horse], b1);
  set_bit(position->boards[horse], g1);
  set_bit(position->boards[horse], b8);
  set_bit(position->boards[horse], g8);

  set_bit(position->boards[king], e1);
  set_bit(position->boards[king], e8);

  set_bit(position->boards[queen], d1);
  set_bit(position->boards[queen], d8);

  set_bit(position->boards[rook], a1);
  set_bit(position->boards[rook], a8);
  set_bit(position->boards[rook], h1);
  set_bit(position->boards[rook], h8);

  set_bit(position->boards[bishop], c1);
  set_bit(position->boards[bishop], f1);
  set_bit(position->boards[bishop], c8);
  set_bit(position->boards[bishop], f8);


}

void empty_position(position *position) {
  for (int c = 0; c < 8; c++) {
    position->boards[c] = 0x0ULL;
  }
  init_castle(position);
  init_enpassant(position);
  init_turn(position);
}

void init_position(position *position) {
  init_boards(position);
  init_square(position);
  init_castle(position);
  init_enpassant(position);
  init_game_list(position);
  init_turn(position);
}

position parse_fen(char *fen) {
  //seperate
//arrays currently used in parse_fen
  int char_pieces[] = {
      ['P'] = pawn,
      ['p'] = pawn,
      ['N'] = horse,
      ['n'] = horse,
      ['B'] = bishop,
      ['b'] = bishop,
      ['R'] = rook,
      ['r'] = rook,
      ['Q'] = queen,
      ['q'] = queen,
      ['K'] = king,
      ['k'] = king};
      
  int char_files[] = {
      ['a'] = a,
      ['b'] = b,
      ['c'] = c,
      ['d'] = d,
      ['e'] = e,
      ['f'] = f,
      ['g'] = g,
      ['h'] = h};

  position position;
  empty_position(&position);
  unsigned long length = strlen(fen);
  int color, piece, sq;
  int offset = 0;

  printf("%s", fen);
  

  //board
  //for ranks 8-1
  for (int r = 7; r >= 0 ; r--) {
    //for files a-h
    for (int f = 0; f < 8; f++) {
      
      if (offset > 0) { 
        offset--;
      }
      else {
        sq = r*8+f;
        //ignore symbol
        if (*fen == '/') fen++;
        //spaces to skip
        if (*fen >= '0' && *fen <= '8') {
          //skip sq's, -1 because skip current sq
          offset = *fen - '0' - 1;
          fen++;
        }
        //piece
        else if ((*fen >= 'a' && *fen <= 'z')||(*fen >= 'A' && *fen <= 'Z')) {
          //white pieces
          if (*fen >= 'A' && *fen <= 'Z') {
            color = white;
            piece = char_pieces[*fen];
          }
          //black pieces
          else {
            color = black;
            piece = char_pieces[*fen];
          }
          set_bit(position.boards[piece], sq);
          set_bit(position.boards[color], sq);
          fen++;
        }
      }
    }
  }

  //skip space
  fen++;

  //color turn
  if (*fen == 'w') {
    color = white;
  }
  else {
    color = black;
  } 
  position.turn = color;
  fen++;

  //skip space
  fen++;

  //castle
  int wk = 0;
  int wq = 0;
  int bk = 0;
  int bq = 0;
  if (*fen != '-') {
    while (*fen != ' ') {
      if (*fen == 'K') wk = 1;
      else if (*fen == 'Q') wq = 1;
      else if (*fen == 'k') bk = 1;
      else if (*fen == 'q') bq = 1;
      fen++;
    }
  }
  //skip -
  else fen++;
  position.castle = encode_castle(wk, wq, bk, bq);

  //skip space
  fen++;

  //enpassant
  int flag = 0;
  int rank = 0;
  piece = 0;
  if (*fen != '-') {
    rank = char_files[*fen];
    flag = 1;
    fen++;
    fen++;
  }
  //skip -
  else fen++;
  position.enpassant = encode_enpassant(flag, rank);

  /*
  //skip space
  fen++;
  */

  //still need to add half and full moves
  return position;
}








