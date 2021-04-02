/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   attack.h   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef ATTACK_H
#define ATTACK_H

#include "bitboards.h"
#include "position.h"

/***************************\
          ATTACKS       
\***************************/

extern U64 ray_attacks[8][64];
extern U64 pawn_attacks[2][64];
extern U64 horse_attacks[64];
extern U64 king_attacks[64];

//directions
enum {
  N, NE, E, SE, S, SW, W, NW
};


//pawn attack
//used to fill up table [2][64] ([color][square])
U64 pawn_attack(int sq, int color);

//horse attack
//used to fill up table [64] ([square])
U64 horse_attack(int sq);

//king attack
//used to fill up table [64] ([square])
U64 king_attack(int sq);


//slider line & ray attacks (rank & file & diag & antidiag)

//rook line & ray attacks (rank & file)
//also used for queen attacks
U64 file_attack(int sq);
U64 north_attack(int sq);
U64 south_attack(int sq);
U64 rank_attack(int sq);
U64 east_attack(int sq);
U64 west_attack(int sq);
U64 rook_attack(int sq);


//bishop line & ray attacks (diag & antidiag)
//also used for queen attacks
U64 diag_attack(int sq);
U64 northeast_attack(int sq);
U64 southwest_attack(int sq);
U64 antidiag_attack(int sq);
U64 northwest_attack(int sq);
U64 southeast_attack(int sq);
U64 bishop_attack(int sq);

//queen attacks (rook & bishop)
U64 queen_attack(int sq);
void init_attack();


/***************************\
           MOVES   
\***************************/

U64 positive_ray_attack(U64 occupied, int dir, int sq);
U64 negative_ray_attack(U64 occupied, int dir, int sq);
U64 get_bishop_attack(U64 occupied, int sq);
U64 get_rook_attack(U64 occupied, int sq);
U64 get_queen_attack(U64 occupied, int sq);

int get_sq(int file, int rank);

#endif