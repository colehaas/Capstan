/*************************************\
 * ================================= *
   \^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
   !%^`$!"\               %!"$%!%\!%
   +\$%^!    position.h    !^$&%?`\>
   \?&%+\!+               &$%\!%$!%$
   $%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef POSITION_H
#define POSITION_H

#include "bitboards.h"


//colors & pieces
enum {
  white, black, pawn, horse, king, queen, rook, bishop
};

enum {
  p, h, k, q, r, b, P, H, K, Q, R, B
};



/***** CASTLING ENCODING FORMAT ******\
  
  wk: white king
  wq: white queen
  bk: black king
  bq: black queen

\*************************************/



//castle encoding macros
#define encode_castle(whiteking, whitequeen, blackking, blackqueen) \
    (whiteking) | (whitequeen << 1) |                               \
        (blackking << 2) | (blackqueen << 3)

#define decode_castle_wk(castle) (castle & 1)
#define decode_castle_wq(castle) ((castle & 2) >> 1)
#define decode_castle_bk(castle) ((castle & 4) >> 2)
#define decode_castle_bq(castle) ((castle & 8) >> 3)

/* void encode_castle(unsigned int wk, unsigned int wq, unsigned int bk, unsigned int bq);
void decode_castle_wk(int castle);
void decode_castle_wq(int castle);
void decode_castle_bk(int castle);
void decode_castle_bq(int castle); */


/***** ENPASSANT ENCODING FORMAT *****\
  f: flag
  r: rank
  frrr
\*************************************/

//probably not useful
/* enum rank
{
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h
}; */

//enpassant encoding macros
#define encode_enpassant(flag, rank) (flag) | (rank << 1)
#define decode_enpassant_flag(enpassant) (enpassant & 1)
#define decode_enpassant_rank(enpassant) ((enpassant & 0xE) >> 1)


typedef struct
{
  //bitboards
  U64 boards[8];
  //squareboard
  unsigned int square[64];  
  //color to move
  unsigned short turn;
  //which castles can done
  unsigned short castle;
  //current enpassantable pawns
  unsigned short enpassant;
  //index for gamelist struct
  unsigned int index;
  
} position;


void init_boards(position *position);
void init_position(position *position);
void parse(position *pos, char *fen);
void printpos(position pos);

#endif
