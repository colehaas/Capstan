/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   chess.c   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#include "generate.h"


/*********************\
* MOVE REPRESENTATION *
\*********************/



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

  //int lsb = bit_scan_forward(bb);
  //int msb = bit_scan_reverse(bb);

  //printf("    lsb index: %d\n\n", lsb);
  //printf("    msb index: %d\n\n", msb);

  }

void printpiecepretty(int color, int piece) {
    char p;
    switch (piece) {
    case(pawn):
      p = 'p';
      break;
    case(horse):
      p = 'n';
      break;
    case(king):
      p = 'k';
      break;
  case(queen):
      p = 'q';
      break;
  case(rook):
    p = 'r';
    break;
  case(bishop):
    p = 'b';
    break;
  }
  if (color == white) {
    printf("<%c>", p);
  }
  else {
    printf("[%c]", p);
  }
}

void printgb(position position) {
  int r, f, color, piece;
  printf("\n    ---  ---  ---  ---  ---  ---  ---  ---\n");
  for (r = 7; r >= 0; r--) {
    for (f = 0; f < 8; f++) {
      int sq = r*8+f;
      //print rank
      if (!f) printf("%d | ", r+1);

      color = get_color_sq(position, sq);
      piece = get_piece_sq(position, sq);
      //check if piece and print
      if (!((color == -1) | (piece == -1))) { 
        printpiecepretty(color, piece);
      }
      //if square is empty
      else {
        U64 sq_bb = 0x0ULL;
        set_bit(sq_bb, sq);
        //if sq is dark or light and print
        if (sq_bb & dark) {
          printf("###");
        }
        else {
          printf("   ");
        }
      }
      printf("  ");
    }
    printf("\n    ---  ---  ---  ---  ---  ---  ---  ---\n");
  }
  printf("     a    b    c    d    e    f    g    h\n\n");
}

void printsq(int sq) {
  char letters[] = "abcdefgh";
  int file = sq % 8;
  char letter = letters[file];
  int number = (sq/8) + 1;
  printf("%s%d", &letter, number);
}

void printcolor(int color) {
  if (color == white) printf("White ");
  else printf("Black ");
}

void printpiece(int piece) {
  switch (piece) {
    case(pawn):
      printf("Pawn ");
      break;
    case(horse):
      printf("Horse ");
      break;
    case(king):
      printf("King ");
      break;
  case(queen):
      printf("Queen ");
      break;
  case(rook):
    printf("Rook ");
    break;
  case(bishop):
      printf("Bishop ");
      break;
  case(8):
    printf("Castle King ");
    break;
  case(9):
    printf("Castle Queen ");
    break;
  case(10):
    printf("Promotion ");
    break;
  }
}

void printmove(int move) {
  int source = decode_source(move);
  int target = decode_target(move);
  int color = decode_color(move);
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);
  int piece = 0;

  printcolor(color);
  //case: Castling
  if ((promotion==0)&(capture==0)&(y==1)) {
    //queen
    if (z==0) {
      piece = 9;
    }
    //king
    else {
      piece = 8;
    }
  }
  //case: Promotion
  else if (promotion==1) {
    piece = 10;
  }
  //case: Normal
  else {
    piece = decode_piece(move);
  }
  printpiece(piece);
  printf(": ");
  printsq(source);
  printf(" -> ");
  printsq(target);
}

void printmoves(move_list moves) {
  for (int c = 0; c < moves.count; c++) {
      printf("(%0.3d)  ", c);
      printmove(moves.moves[c]);
      printf("\n");
  }
}


int promotion_piece(int y, int z) {
  if (y == 0) {
    if (z == 0) return horse;
    else return bishop;
  }
  else {
    if (z == 0) return rook;
    else return queen;
  }
  return 0;
}


//function to get all pseudo legal moves for a color
move_list generate_moves(position position) {
  
  move_list list;
  list.count = 0;
  
  U64 occupied = get_occupied(position.boards[white], position.boards[black]);
  U64 moves, attacks, temp;
  U64 enpassantbb = 0x0ULL;
  int sq, target;
  int color = position.turn;
  int opposite = get_opposite(color);

  int enpassantable = decode_enpassant_flag(position.enpassant);
  int enpassant = 0;
  int enpassant_sq;
  
  if (enpassantable) {
    enpassant = decode_enpassant_rank(position.enpassant);
  }
  
  int castle = position.castle;

  
  //generate castling moves
  //check if can castle and open to castle
  //white
  if (color == white) {
    if ((decode_castle_wk(castle)) && ((wk_castle_open & ~occupied)==wk_castle_open)) {
      list.moves[list.count++] = encode_move(0, 0, 0, 0, 1, 1, 0, color);
    }
    if ((decode_castle_wq(castle)) && ((wq_castle_open & ~occupied)==wq_castle_open)) {
      list.moves[list.count++] = encode_move(0, 0, 0, 0, 1, 0, 0, color);
    }
  }
  //black
  else {
    if ((decode_castle_bk(castle)) && ((bk_castle_open & ~occupied)==bk_castle_open)) {
      list.moves[list.count++] = encode_move(0, 0, 0, 0, 1, 1, 0, color);
    }
    if ((decode_castle_bq(castle)) && ((bq_castle_open & ~occupied)==bq_castle_open)) {
      list.moves[list.count++] = encode_move(0, 0, 0, 0, 1, 0, 0, color);
    }
  }

  //generate pawns moves
  U64 pawns = get_bb(position.boards[color], position.boards[pawn]);
  while (pawns) {
    
    moves = 0x0ULL;
    sq = bit_scan_forward(pawns);
    set_bit(moves, sq);

    //white
    if (color == white) {

      //single pushes
      if ((moves << 8) & ~occupied) {
        list.moves[list.count++] = encode_move(sq, (sq+8), 0, 0, 0, 0, pawn, color);
        //double pushes
        if ((moves & rank_2) && ((moves << 16) & ~occupied)) {
            list.moves[list.count++] = encode_move(sq, (sq+16), 0, 0, 0, 1, pawn, color);
        }
        //promotions & promotion captures
        if (moves & rank_7) {
          list.moves[list.count++] = encode_move(sq, (sq+8), 1, 0, 0, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq+8), 1, 0, 0, 1, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq+8), 1, 0, 1, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq+8), 1, 0, 1, 1, pawn, color);
        }
      }

      //attacks
      attacks = pawn_attacks[color][sq];
      attacks &= position.boards[opposite];
      while (attacks) {
        target = bit_scan_forward(attacks);
        //promotion capture
        if (moves & rank_7) {
          list.moves[list.count++] = encode_move(sq, target, 1, 1, 0, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, target, 1, 1, 0, 1, pawn, color);
          list.moves[list.count++] = encode_move(sq, target, 1, 1, 1, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, target, 1, 1, 1, 1, pawn, color);
        }
        //normal capture
        else {
          list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, pawn, color);
        }
        pop_bit(attacks, target);
      }
      //enpassant
      //nested because there cant't be a piece between double push
      if (enpassantable && (moves & rank_5)) {
        //square to be attacked
        enpassant_sq = get_sq(enpassant, 6);
        enpassantbb = 0x0ULL;
        set_bit(enpassantbb, enpassant_sq);
        attacks = pawn_attacks[color][sq];
        attacks &= (enpassantbb & ~occupied);
        while(attacks) {
          target = bit_scan_forward(attacks);
          list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 1, pawn, color);
          pop_bit(attacks, target);
        }
      }
    }

    //black
    else {
      //single pushes
      if ((moves >> 8) & ~occupied) {
        list.moves[list.count++] = encode_move(sq, (sq-8), 0, 0, 0, 0, pawn, color);
        //double pushes
        //nested because there cant't be a piece between double push
        if ((moves & rank_7) && ((moves >> 16) & ~occupied)) {
          list.moves[list.count++] = encode_move(sq, (sq-16), 0, 0, 0, 1, pawn, color);
        }
        //promotions
        if (moves & rank_2) {
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 0, 0, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 0, 0, 1, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 0, 1, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 0, 1, 1, pawn, color);
        }
      }

      //attacks
      attacks = pawn_attacks[color][sq];
      attacks &= position.boards[opposite];
      while (attacks) {
        target = bit_scan_forward(attacks);
        //promotion attack
        if (moves & rank_2) {
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 1, 0, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 1, 0, 1, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 1, 1, 0, pawn, color);
          list.moves[list.count++] = encode_move(sq, (sq-8), 1, 1, 1, 1, pawn, color);
        }
        //normal attack
        else {
          list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, pawn, color);
        }
        pop_bit(attacks, target);
      }

      //enpassant
      if (enpassantable && (moves & rank_4)) {
        //square to be attacked
        enpassant_sq = get_sq(enpassant, 3);
        enpassantbb = 0x0ULL;
        set_bit(enpassantbb, enpassant_sq);
        attacks = pawn_attacks[color][sq];
        attacks &= (enpassantbb & ~occupied);
        while(attacks) {
          target = bit_scan_forward(attacks);
          list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 1, pawn, color);
          pop_bit(attacks, target);
        }
      }
    }

  pop_bit(pawns, sq);
  }

  //generate horse moves
  U64 horses = get_bb(position.boards[color], position.boards[horse]);
  while (horses) {
    sq = bit_scan_forward(horses);

    moves = horse_attacks[sq];
    moves &= ~position.boards[color];

    temp = moves & position.boards[opposite];
    moves &= ~position.boards[opposite];

    while (temp) {
      target = bit_scan_forward(temp);
      list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, horse, color);
      pop_bit(temp, target);
    }
    
    while (moves) {
      target = bit_scan_forward(moves);
      list.moves[list.count++] = encode_move(sq, target, 0, 0, 0, 0, horse, color);
      pop_bit(moves, target);
    }
    pop_bit(horses, sq);
  }

  //generate king moves
  U64 kings = get_bb(position.boards[color], position.boards[king]);
  while (kings) {
    sq = bit_scan_forward(kings);

    moves = king_attacks[sq];
    moves &= ~position.boards[color];

    temp = moves & position.boards[opposite];
    moves &= ~position.boards[opposite];

    while (temp) {
      target = bit_scan_forward(temp);
      list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, king, color);
      pop_bit(temp, target);
    }
    
    while (moves) {
      target = bit_scan_forward(moves);
      list.moves[list.count++] = encode_move(sq, target, 0, 0, 0, 0, king, color);
      pop_bit(moves, target);
    }
    pop_bit(kings, sq);
  }

  //generate queen moves
  U64 queens = get_bb(position.boards[color], position.boards[queen]);
  while (queens) {
  sq = bit_scan_forward(queens);

  moves = get_queen_attack(occupied, sq);
  moves &= ~position.boards[color];

  temp = moves & position.boards[opposite];
  moves &= ~position.boards[opposite];

  while (temp) {
      target = bit_scan_forward(temp);
      pop_bit(temp, target);
      list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, queen, color);
    }
    
    while (moves) {
      target = bit_scan_forward(moves);
      list.moves[list.count++] = encode_move(sq, target, 0, 0, 0, 0, queen, color);
      pop_bit(moves, target);
    }
  pop_bit(queens, sq);
  }
  
  //generate rook moves
  U64 rooks = get_bb(position.boards[color], position.boards[rook]);
  while (rooks) {
  sq = bit_scan_forward(rooks);
  
  moves = get_rook_attack(occupied, sq);
  moves &= ~position.boards[color];

  temp = moves & position.boards[opposite];
  moves &= ~position.boards[opposite];

  while (temp) {
      target = bit_scan_forward(temp);
      list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, rook, color);
      pop_bit(temp, target);
    }
    
    while (moves) {
      target = bit_scan_forward(moves);
      list.moves[list.count++] = encode_move(sq, target, 0, 0, 0, 0, rook, color);
      pop_bit(moves, target);
    }
  pop_bit(rooks, sq);
  }

  //generate bishop moves
  U64 bishops = get_bb(position.boards[color], position.boards[bishop]);
  while (bishops) {
    sq = bit_scan_forward(bishops);

    moves = get_bishop_attack(occupied, sq);
    moves &= ~position.boards[color];

    temp = moves & position.boards[opposite];
    moves &= ~position.boards[opposite];
    
    while (temp) {
      target = bit_scan_forward(temp);
      list.moves[list.count++] = encode_move(sq, target, 0, 1, 0, 0, bishop, color);
      pop_bit(temp, target);
    }
    

    while (moves) {
      target = bit_scan_forward(moves);
      list.moves[list.count++] = encode_move(sq, target, 0, 0, 0, 0, bishop, color);
      pop_bit(moves, target);
    }
    pop_bit(bishops, sq);
  }
  
  return list;
}
