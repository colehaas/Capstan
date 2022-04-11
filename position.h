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

#define MAXMOVES 256
#define INFINITE 100000

//colors & pieces
enum {
  white, black, pawn, horse, king, queen, rook, bishop
};

enum {
  p, h, k, q, r, b, P, H, K, Q, R, B
};

//probably not useful
// enum rank { a, b, c, d, e, f, g, h};

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

/***** ENPASSANT ENCODING FORMAT *****\
  f: flag
  r: rank
  frrr
\*************************************/

//enpassant encoding macros
#define encode_enpassant(flag, rank) (flag) | (rank << 1)
#define decode_enpassant_flag(enpassant) (enpassant & 1)
#define decode_enpassant_rank(enpassant) ((enpassant & 0xE) >> 1)



// this struct stores the info necessary to unmake or undo one move. 
typedef struct {
  //move
  unsigned int move;
  //hash key
  U64 hash_key;
  //which castles can be done
  unsigned short castle;
  //current enpassantable pawns
  unsigned short enpassant;
  //piece captured
  unsigned short captured;
} undo_list;


typedef struct {
    U64 hash_key;
    int best_move;
    //int depth
    //int score
    //int node_type
    //int age
} hash_node;

typedef struct {
    hash_node *hash;
    int num_nodes;
} hash_table;

typedef struct {
  
  //bitboards
  U64 boards[8];
  //squareboard
  unsigned int square[64]; 

  //hash key
  U64 hash_key;
  //hash table
  hash_table table;

  //color to move
  unsigned short turn;
  //which castles can be done
  unsigned short castle;
  //current enpassantable pawns
  unsigned short enpassant;
  //number of plys and index for undolist
  unsigned int index;
  
  //undo list for unmake
  undo_list undo[MAXMOVES];


} position;


void init_boards(position *position);
void init_position(position *position);
void parse(position *pos, char *fen);
void printpos(position pos);

//from hash.h

extern U64 hash_values[13][64];

U64 xorshift(U64);
void init_hash();

#endif
