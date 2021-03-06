
/***********************************************\
 * =========================================== *


              CHESS ENGINE V0
                 by 16haas

 * =========================================== *
\***********************************************/


//name ideas: Capstan 

//system headers
#include <stdio.h>

//define bitboard datatype
#define U64 unsigned long long


//ENUMERATIONS

//bitboard squares (Little-Endian-Rank-File)
enum {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

//color
enum {
  white, black
};

enum {
  N, NE, E, SE, S, SW, W, NW
};





//useful bitboards

//files
U64 a_file = 0x0101010101010101ULL;
U64 b_file = 0x0202020202020202ULL;
U64 g_file = 0x4040404040404040ULL;
U64 h_file = 0x8080808080808080ULL;

//ranks
U64 rank_1 = 0xffULL;

//others
U64 border = 0xff818181818181ffULL;

//not file functions
U64 not_a() {return ~a_file;}
U64 not_h() {return ~h_file;}

U64 not_ab() {return ~(a_file | b_file);}
U64 not_gh() {return ~(g_file | h_file);}

/***********************************************\
 * =========================================== *
                Bit Manipulations
 * =========================================== *
\***********************************************/

//bit macros
#define set_bit(bb, sq) (bb |= (1ULL << sq))
#define get_bit(bb, sq) (bb & (1ULL << sq))
#define pop_bit(bb, sq) (get_bit(bb, sq) ? bb ^= (1ULL << sq) : 0)

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

void fill_pawn_attack() {
  U64 attacks[2][64];
  for (int sq = 0; sq < 64; sq++) {
    attacks[white][sq] = pawn_attack(sq, white);
    attacks[black][sq] = pawn_attack(sq, black);
  }
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

void fill_horse_attack() {
  U64 attacks[64];
  for (int sq = 0; sq < 64; sq++) {
    attacks[sq] = horse_attack(sq);
  }
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

void fill_king_attack() {
  U64 attacks[64];
  for (int sq = 0; sq < 64; sq++) {
    attacks[sq] = king_attack(sq);
  }
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

void fill_ray_attack() {
  U64 attacks[8][64];
  for (int sq = 0; sq < 64; sq++) {
    attacks[N][sq] = north_attack(sq);
    attacks[NE][sq] = northeast_attack(sq);
    attacks[E][sq] = east_attack(sq);
    attacks[SE][sq] = southeast_attack(sq);
    attacks[S][sq] = south_attack(sq);
    attacks[SW][sq] = southwest_attack(sq);
    attacks[W][sq] = west_attack(sq);
    attacks[NW][sq] = northwest_attack(sq);
  }
}

U64 beyond(U64 bb) {
  return bb & ~border;
}

U64 behind() {
  U64 bb = 0ULL;
  return bb;
}


/***********************************************\
 * =========================================== *
                 Main Driver
 * =========================================== *
\***********************************************/

int main()  {
    printf("\n\nhello\n\n");

    U64 bb = 0ULL;

    //bb = pawn_attack(h3, black);
    //bb = horse_attack(g2);
    //bb = king_attack(e6);
    //bb = rank_attack(e3);
    //bb = file_attack(a3);
    //bb = rook_attack(f5);
    //bb = diag_attack(f5);
    //bb = antidiag_attack(f5);
    //bb = bishop_attack(g2);
    bb = northwest_attack(d5);



    printbb(bb);

    printf("\n\n\n");
    return 0;
}