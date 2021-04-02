/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   attack.c   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#include "attack.h"

/***************************\
			    ATTACKS       
\***************************/

U64 ray_attacks[8][64];
U64 pawn_attacks[2][64];
U64 horse_attacks[64];
U64 king_attacks[64];


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

