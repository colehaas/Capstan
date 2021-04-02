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

/***** CASTLING ENCODING FORMAT ******\
  
  wk: white king
  wq: white queen
  bk: black king
  bq: black queen

\*************************************/

enum castle
{
    wk = 1,
    wq = 2,
    bk = 4,
    bq = 8
};

//castle encoding macros
#define encode_castle(whiteking, whitequeen, blackking, blackqueen) \
    (whiteking) | (whitequeen << 1) |                               \
        (blackking << 2) | (blackqueen << 3)

#define decode_castle_wk(castle) (castle & wk)
#define decode_castle_wq(castle) ((castle & wq) >> 1)
#define decode_castle_bk(castle) ((castle & bk) >> 2)
#define decode_castle_bq(castle) ((castle & bq) >> 3)

/***** ENPASSANT ENCODING FORMAT *****\
  f: flag
  r: rank
  frrr
\*************************************/

//probably not useful
enum rank
{
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h
};

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
    //which castles can done
    unsigned short castle;
    //current enpassantable pawns
    unsigned short enpassant;
    //color to move
    unsigned short turn;
} position;

void init_square(position *position);
void init_enpassant(position *position);
void init_castle(position *position);
void init_turn(position *position);
void init_boards(position *position);
void empty_position(position *position);
void init_position(position *position);
position parse_fen(char *fen);

#endif