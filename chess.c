/*****************************************************************************\
 * ========================================================================= *    
  &^%^%%^%?!/|!^&)!?<$%^$$/?^!%^&\`$^&!\%+\$!%^\?^<?%}$\%$^&>!$%^<>?^&+\^%^&
  ?^&\$`%$%>"&^%^%^$!%+\^&%!%$/?^&+\^&%!\%$/?^&\$&\%$/?^!%^&%+\$!%^\?&%+\!^?
  ^%&`\}/\`$^&!\%$/\&+$+?\$%%^                 +$%^`%+$%^&!+?\$%%^!%\+$<%^&$
  \`$%\`?^%?<}%^&$%^>?%^&       C A P S T A N       `\%$`\$%$>^<?%}$%$^%%^!%
  !\$/\`%`\%$^&!<!%^&>    a  c h e s s  e n g i n e     $%+\`?\$%^!%\+$%^`%!
  &%^$[$!\?+\%^$\!^%^$                                  !%$!/?^&+\^&!+?\$%!?
  \$%^+%$%>"&^%^\$^`$$^&          by 16haas           &!^!`&<&>!`%\\$!\'$\`$
  \%$`/&^!}?!%}|%>!<"~|}!$\                        $^%/%&^%$`\%$!!%^"$%\`$`%
  `%$^?\$^\&^$`%?\%$%>"&^%^%%^%?!/|_^&)!?<$%^$&^<&>!`%\$!\!`%^$%^$%`\]!!$\`/
  %>"&^%^%^$!%+\^&%!%&!\%$%\?+\%$%\?+\%$^!$/$$/?^!%^&\`$^&!\%`\%$^&>!+\$!%^&
 * ========================================================================= *
\******************************************************************************/

/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   chess.c   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

/***************************\
 * ======================= *
 *   SECTION 1: HEADERS    *
 * ======================= *
\***************************/

//system headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//define bitboard datatype
#define U64 unsigned long long


/**********************\
* BOARD REPRESENTATION *
\**********************/

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

//colors & pieces
enum {
  white, black, pawn, horse, king, queen, rook, bishop
};

//one board for each colors & pieces

/***** CASTLING ENCODING FORMAT ******\
  
  wk: white king
  wq: white queen
  bk: black king
  bq: black queen

\*************************************/

enum castle {
  wk = 1, wq = 2, bk = 4, bq = 8
};

#define encode_castle(whiteking, whitequeen, blackking, blackqueen) \
  (whiteking) | (whitequeen << 1) | \
  (blackking << 2) | (blackqueen << 3) 

#define decode_castle_wk(castle) (castle&wk)
#define decode_castle_wq(castle) ((castle&wq) >> 1)
#define decode_castle_bk(castle) ((castle&bk) >> 2)
#define decode_castle_bq(castle) ((castle&bq) >> 3)

/***** ENPASSANT ENCODING FORMAT *****\
  f: flag
  r: rank
  frrr
\*************************************/

//probably not useful
enum rank {
  a, b, c, d, e, f, g, h
};

#define encode_enpassant(flag, rank) (flag) | (rank << 1)

#define decode_enpassant_flag(enpassant) (enpassant&1)
#define decode_enpassant_rank(enpassant) ((enpassant&0xE) >> 1)


typedef struct {
  U64 boards[8];
  //which castles can done 
  unsigned short castle;
  //current enpassantable pawns
  unsigned short enpassant;
  //color to move
  unsigned short turn;
} position;

void init_enpassant(position *position) {
  position->enpassant = encode_enpassant(0, 0);
}

void init_castle(position *position) {
  position->castle = encode_castle(1, 1, 1, 1);
}

void init_turn(position *position) {
  position->turn = white;
}


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
  1000: knight promotion
  1001: bishop promotion
  1010: rook promotion
  1011: queen promotion
  ______________________________
  1100: knight promotion capture
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
  int moves[256];
  int count;
} move_list;


/*********************\
*  USEFUL BITBOARDS  *
\*********************/

//files
U64 a_file = 0x0101010101010101ULL;
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
U64 wk_castle_open = 0x60ULL;
U64 wq_castle_open = 0x0EULL;
U64 bk_castle_open = 0x6000000000000000ULL;
U64 bq_castle_open = 0x0E00000000000000ULL;

//others
U64 full = 0xffffffffffffffffULL;
U64 border = 0xff818181818181ffULL;
U64 dark = 0xAA55AA55AA55AA55ULL;


//not file functions
U64 not_a() {return ~a_file;}
U64 not_h() {return ~h_file;}

U64 not_ab() {return ~(a_file | b_file);}
U64 not_gh() {return ~(g_file | h_file);}

//directions
enum {
  N, NE, E, SE, S, SW, W, NW
};


/***********************************************\
 * ===========================================*
                Bit Manipulations
 * =========================================== *
\***********************************************/

//bit macros
#define set_bit(bb, sq) (bb |= (1ULL << sq))
#define get_bit(bb, sq) (bb & (1ULL << sq))
#define pop_bit(bb, sq) (get_bit(bb, sq) ? bb ^= (1ULL << sq) : 0)

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

//color being attacked
int is_attacked(position position, int sq) {
  U64 bb_sq = 0x0ULL;
  set_bit(bb_sq, sq);
  move_list moves = generate_moves(position);
  
  
}

//for make function
int get_rank(int sq) {
  return (sq % 8);
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



U64 get_bb(U64 color, U64 piece) {
  return (color & piece);
}

U64 get_occupied(U64 white, U64 black) {
  return (white | black);
}

void move_piece(position *position, int color, int piece, int sq, int target_sq) {
  pop_bit(position->boards[color], sq);
  pop_bit(position->boards[piece], sq);
  set_bit(position->boards[color], target_sq);
  set_bit(position->boards[piece], target_sq);
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
  if (color == white ) return black;
  else return white;
}

/***************************\
          ATTACKS       
\***************************/


//pawn attack
//used to fill up table [2][64] ([color][square])
U64 pawn_attack(int sq, int color) {
  U64 bb = 0ULL;
  U64 attack = 0ULL;
  set_bit(bb, sq);

  if (color == white) {
    attack |= ((bb << 7) & not_h());
    attack |= ((bb << 9) & not_a());
  }
  else {
    attack |= ((bb >> 7) & not_a());
    attack |= ((bb >> 9) & not_h());
  }
  return attack;
}

//horse attack
//used to fill up table [64] ([square])
U64 horse_attack(int sq) {
  U64 attack = 0ULL;
  U64 bb = 0ULL;
  set_bit(bb, sq);

  //each horse direction starting with NNE going clockwise
  attack |= ((bb << 17) & not_a());
  attack |= ((bb << 10) & not_ab());
  attack |= ((bb >> 6) & not_ab());
  attack |= ((bb >> 15) & not_a());
  attack |= ((bb >> 17) & not_h());
  attack |= ((bb >> 10) & not_gh());
  attack |= ((bb << 6) & not_gh());
  attack |= ((bb << 15) & not_h());

  return attack;
}

//king attack
//used to fill up table [64] ([square])
U64 king_attack(int sq) {
  U64 attack = 0ULL;
  U64 bb = 0ULL;
  set_bit(bb, sq);
  
  attack |= ((bb >> 8)); //S
  attack |= ((bb << 8)); //N

  attack |= (((bb << 1)|(bb << 9)|(bb >> 7)) & not_a()); //E
  attack |= (((bb >> 1)|(bb << 7))|(bb >> 9) & not_h()); //W

  return attack;
}


//slider line & ray attacks (rank & file & diag & antidiag)

//rook line & ray attacks (rank & file)
//also used for queen attacks

U64 file_attack(int sq) {
  return a_file << (sq & 7);
}

U64 north_attack(int sq) {
  return file_attack(sq) & ((-2ULL) << sq);
}

U64 south_attack(int sq) {
  return file_attack(sq) & (((1ULL) << sq) - 1ULL);
}

U64 rank_attack(int sq) {
  return rank_1 << (sq & 56);
}

U64 east_attack(int sq) {
  return rank_attack(sq) & ((-2ULL) << sq);
}

U64 west_attack(int sq) {
  return rank_attack(sq) & (((1ULL) << sq) - 1ULL);
}

U64 rook_attack(int sq) {
  return file_attack(sq) ^ rank_attack(sq);
}


//bishop line & ray attacks (diag & antidiag)
//also used for queen attacks
U64 diag_attack(int sq) {
  U64 maindia = 0x8040201008040201ULL;
  int diag =8*(sq & 7) - (sq & 56);
  int nort = -diag & ( diag >> 31);
  int sout =  diag & (-diag >> 31);
  return (maindia >> sout) << nort;
}

U64 northeast_attack(int sq) {
  return diag_attack(sq) & ((-2ULL) << sq);
}

U64 southwest_attack(int sq) {
  return diag_attack(sq) & (((1ULL) << sq) - 1ULL);
}

U64 antidiag_attack(int sq) {
  U64 maindia = 0x0102040810204080ULL;
  int diag =56- 8*(sq&7) - (sq&56);
  int nort = -diag & ( diag >> 31);
  int sout =  diag & (-diag >> 31);
  return (maindia >> sout) << nort;
}

U64 northwest_attack(int sq) {
  return antidiag_attack(sq) & ((-2ULL) << sq);
}

U64 southeast_attack(int sq) {
  return antidiag_attack(sq) & (((1ULL) << sq) - 1ULL);
}

U64 bishop_attack(int sq) {
  return diag_attack(sq) ^ antidiag_attack(sq);
}

//queen attacks (rook & bishop)
U64 queen_attack(int sq) {
  return rook_attack(sq) | bishop_attack(sq);
}


U64 ray_attacks[8][64];
U64 pawn_attacks[2][64];
U64 horse_attacks[64];
U64 king_attacks[64];

void init_attack() {

  for (int sq = 0; sq < 64; sq++) {

    ray_attacks[N][sq] = north_attack(sq);
    ray_attacks[NE][sq] = northeast_attack(sq);
    ray_attacks[E][sq] = east_attack(sq);
    ray_attacks[SE][sq] = southeast_attack(sq);
    ray_attacks[S][sq] = south_attack(sq);
    ray_attacks[SW][sq] = southwest_attack(sq);
    ray_attacks[W][sq] = west_attack(sq);
    ray_attacks[NW][sq] = northwest_attack(sq);


    pawn_attacks[white][sq] = pawn_attack(sq, white);
    pawn_attacks[black][sq] = pawn_attack(sq, black);


    horse_attacks[sq] = horse_attack(sq);
    king_attacks[sq] = king_attack(sq);


  }
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
  init_castle(position);
  init_enpassant(position);
  init_turn(position);
}

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
  ['k'] = king
};

int char_files[] = {
  ['a'] = a,
  ['b'] = b,
  ['c'] = c,
  ['d'] = d,
  ['e'] = e,
  ['f'] = f,
  ['g'] = g,
  ['h'] = h
};


position parse_fen(char *fen) {
  //seperate

  position position;
  empty_position(&position);
  unsigned long length = strlen(fen);
  int color, piece, sq;
  int offset = 0;

  printf("%s", fen);
  printf("\n%lu\n\n", length);

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

/***************************\
           MOVES   
\***************************/

U64 positive_ray_attack(U64 occupied, int dir, int sq) {
  U64 attack = ray_attacks[dir][sq];
  U64 blocker = ((attack & occupied) | 0x8000000000000000ULL);
  sq = bit_scan_forward(blocker);
  return attack ^ ray_attacks[dir][sq];
}

U64 negative_ray_attack(U64 occupied, int dir, int sq) {
  U64 attack = ray_attacks[dir][sq];
  U64 blocker = ((attack & occupied) | 0x1ULL);
  sq = bit_scan_reverse(blocker);
  return attack ^ ray_attacks[dir][sq];
}

U64 get_bishop_attack(U64 occupied, int sq) {
  
  U64 attacks = 0x0ULL;
  
  attacks = positive_ray_attack(occupied, NE, sq);
  attacks |= positive_ray_attack(occupied, NW, sq);
  attacks |= negative_ray_attack(occupied, SE, sq);
  attacks |= negative_ray_attack(occupied, SW, sq);

  return attacks;
}

U64 get_rook_attack(U64 occupied, int sq) {
  
  U64 attacks = 0x0ULL;
  
  attacks = positive_ray_attack(occupied, N, sq);
  attacks |= positive_ray_attack(occupied, E, sq);
  attacks |= negative_ray_attack(occupied, S, sq);
  attacks |= negative_ray_attack(occupied, W, sq);

  return attacks;
}

U64 get_queen_attack(U64 occupied, int sq) {
  return (get_bishop_attack(occupied, sq) | get_rook_attack(occupied, sq));
}

int get_sq(int file, int rank) {
  return ( ((rank-1) * 8) + file);
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

/***************************\
       MAKE & UNMAKE   
\***************************/

position make(position from, int move) {
  position to = from;
  int source = decode_source(move);
  int target = decode_target(move);
  int piece = decode_piece(move);
  int color = decode_color(move);
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);
  int capture_piece;


  if (promotion==0) {
    if (capture==0) {
      //quiet and double push 000*
      if (y==0) {
        pop_bit(to.boards[color], source);
        pop_bit(to.boards[piece], source);
        set_bit(to.boards[color], target);
        set_bit(to.boards[piece], target);
        //double push 0001
        if (z==1) to.enpassant = encode_enpassant(1, get_rank(target));
      }
      //castle 001*
      else {
        //king castle 0011
        if (z==1) {
          //white
          if (color == white) {
            //king e1->g1, rook h1->f1
            pop_bit(to.boards[king], e1);
            pop_bit(to.boards[white], e1);
            set_bit(to.boards[king], g1);
            set_bit(to.boards[white], g1);

            pop_bit(to.boards[rook], h1);
            pop_bit(to.boards[white], h1);
            set_bit(to.boards[rook], f1);
            set_bit(to.boards[white], f1);
          }
          //black
          else {
            //king e8->g8, rook h8->f8
            pop_bit(to.boards[king], e8);
            pop_bit(to.boards[black], e8);
            set_bit(to.boards[king], g8);
            set_bit(to.boards[black], g8);

            pop_bit(to.boards[rook], h8);
            pop_bit(to.boards[black], h8);
            set_bit(to.boards[rook], f8);
            set_bit(to.boards[black], f8);
          }
        }
        //queen castle 0010
        else {
          //white
          if (color == white) {
            //king e1->c1, rook a1->d1
            pop_bit(to.boards[king], e1);
            pop_bit(to.boards[white], e1);
            set_bit(to.boards[king], c1);
            set_bit(to.boards[white], c1);

            pop_bit(to.boards[rook], a1);
            pop_bit(to.boards[white], a1);
            set_bit(to.boards[rook], d1);
            set_bit(to.boards[white], d1);
          }
          //black
          else {
            //king e8->c8, rook a8->d8
            pop_bit(to.boards[king], e8);
            pop_bit(to.boards[black], e8);
            set_bit(to.boards[king], c8);
            set_bit(to.boards[black], c8);

            pop_bit(to.boards[rook], a8);
            pop_bit(to.boards[black], a8);
            set_bit(to.boards[rook], d8);
            set_bit(to.boards[black], d8);
          }
        }
      }
    }
    //capture 01**
    else {
      capture_piece = get_piece_sq(from, target);
      pop_bit(to.boards[color], source);
      pop_bit(to.boards[piece], source);

      pop_bit(to.boards[get_opposite(color)], target);
      pop_bit(to.boards[capture_piece], target);

      set_bit(to.boards[color], target);
      set_bit(to.boards[piece], target);
    }
  }
  //promotions
  else {

  }
  to.turn = get_opposite(from.turn);
  return to;
}

void play(position position) {
  int move = 0;
  int color = position.turn;
  move_list moves;
  while (move != -1) {

    //print board and moves
    printf("\n\n");
    printcolor(color);
    printf("To Move:\n\n");
    printgb(position);
    moves = generate_moves(position);
    printmoves(moves);

    //get move from command line
    printf("\n\nEnter Move Number: ");
    scanf("%d", &move);


    if (move != -1) {
      //make move
      position = make(position, moves.moves[move]);
      //switch color
      color = get_opposite(color);
    }

  }
}



/***********************************************\
 * =========================================== *
                 Main Driver
 * =========================================== *
\***********************************************/

int main() {
    printf("\n\nhello\n\n");

    position position;

    init_position(&position);
    init_attack();
/* 
    move_piece(&position, white, pawn, d2, d4);
    move_piece(&position, black, pawn, d7, d5);
    move_piece(&position, white, bishop, c1, f4);
    move_piece(&position, black, pawn, c7, c4);
    move_piece(&position, white, pawn, h2, h4);
    move_piece(&position, black, pawn, a7, b6);
    move_piece(&position, white, pawn, a2, a7);
*/

    position = parse_fen("rn1qkb1r/pp2pppp/5n2/3p1b2/3P4/2N1P3/PP3PPP/R1BQKBNR w KQkq - 0 1");
    printf("\n\n%d%d\n\n", decode_enpassant_flag(position.enpassant), decode_enpassant_rank(position.enpassant));
    play(position);
    
/*
    //printbb(get_occupied(position.boards[white], position.boards[black]));
    printgb(position);
    
    move_list moves = generate_moves(position, white);
    printmoves(moves);
    
*/
  
    printf("\n\n\n");
    return 0;
}