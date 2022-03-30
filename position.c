/*************************************\
 * ================================= *
   \^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
   !%^`$!"\               %!"$%!%\!%
   +\$%^!    position.c    !^$&%?`\>
   \?&%+\!+               &$%\!%$!%$
   $%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#include "position.h"


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

void init_position(position *pos) {
  for (int c = 0; c < 8; c++) {
    pos->boards[c] = 0x0ULL;
  }
    for (int sq = 0; sq < 64; sq++) {
    pos->square[sq] = 0;
  }
  pos->turn = white;
  pos->castle = encode_castle(1, 1, 1, 1);
  pos->enpassant = encode_enpassant(0, 0);
  pos->index = 0;

  pos->undo->move = 0;
  pos->undo->castle = encode_castle(1, 1, 1, 1);
  pos->undo->enpassant = encode_enpassant(0, 0);
  pos->undo->captured = 0;
}

void parse(position *pos, char *fen) {
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
      ['a'] = 0,
      ['b'] = 1,
      ['c'] = 2,
      ['d'] = 3,
      ['e'] = 4,
      ['f'] = 5,
      ['g'] = 6,
      ['h'] = 7};

  init_position(pos);

  int color, piece, sq;
  int offset = 0;

  //convert fen into readable char array for strtok
  size_t size = strlen(fen) + 1;
  char *c = malloc(size);
  strcpy(c, fen);

  char delim[] = " ";

  //first call for strtok
	char *ptr = strtok(c, delim);

  //set boards
  //would love to make this cleaner
	if (ptr != NULL) {
    
    size = strlen(ptr) + 1;
    char *board = malloc(size);
    strcpy(board, ptr);
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
          if (*board == '/') board++;
          //spaces to skip
          if (*board >= '0' && *board <= '8') {
            //skip sq's, -1 because skip current sq
            offset = *board - '0' - 1;
            board++;
          }
          //piece
          else if ((*board >= 'a' && *board <= 'z')||(*board >= 'A' && *board <= 'Z')) {
            //white pieces
            if (*board >= 'A' && *board <= 'Z') {
              color = white;
              piece = char_pieces[(int)*board];
            }
            //black pieces
            else {
              color = black;
              piece = char_pieces[(int)*board];
            }
            set_bit(pos->boards[piece], sq);
            set_bit(pos->boards[color], sq);
            board++;
          }
        }
      }
    }
  }

  //set color
  if (ptr != NULL) {
    ptr = strtok(NULL, delim);
    if (*ptr == 'b') pos->turn = black;
    else pos->turn = white;
	}

  //set castling
  if (ptr != NULL) {
    ptr = strtok(NULL, delim);
    size = strlen(ptr) + 1;
    char *castle = malloc(size);
    strcpy(castle, ptr);

    int wk= 0, wq = 0, bk = 0, bq = 0;
  
    if (*castle != '-') {
      for (int c = 0; c < 4; c++) {
        if (*castle == 'K') wk = 1;
        else if (*castle == 'Q') wq = 1;
        else if (*castle == 'k') bk = 1;
        else if (*castle == 'q') bq = 1;
        castle++;
      }
    }
    pos->castle = encode_castle(wk, wq, bk, bq);
  }

  //set enpassant
  if (ptr != NULL) {
    ptr = strtok(NULL, delim);
    if (*ptr != '-') {
      pos->enpassant = encode_enpassant(1, char_files[(int)*ptr]);
    }
    else {
      pos->enpassant = encode_enpassant(0, 0);
    }
  }

}

void printpos(position pos) {
  
  printf("\nTurn: ");
  if (pos.turn) printf("Black");
  else printf("White");

  printf("\nCastling: ");
  int castle = pos.castle;
  if (decode_castle_wk(castle)) printf("K");
  if (decode_castle_wq(castle)) printf("Q");
  if (decode_castle_bk(castle)) printf("k");
  if (decode_castle_bq(castle)) printf("q");

  printf("\nEnpassant: ");
  if (decode_enpassant_flag(pos.enpassant)) {
    printf("%d", decode_enpassant_rank(pos.enpassant));
  
  }

}





