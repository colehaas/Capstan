
/***********************************************\
 * =========================================== *


                   CAPSTAN

                a chess engine
                  by 16haas

 * =========================================== *
\***********************************************/


//name ideas: Capstan 

//system headers
#include <stdio.h>
#include <math.h>

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

//colors & pieces
enum {
  white, black, pawn, horse, king, queen, rook, bishop
};

enum {
  N, NE, E, SE, S, SW, W, NW
};

//board representation

U64 boards[8];

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
 * ===========================================*
                Bit Manipulations
 * =========================================== *
\***********************************************/

//bit macros
#define set_bit(bb, sq) (bb |= (1ULL << sq))
#define get_bit(bb, sq) (bb & (1ULL << sq))
#define pop_bit(bb, sq) (get_bit(bb, sq) ? bb ^= (1ULL << sq) : 0)

void move_piece(int color, int piece, int sq, int target_sq) {
  pop_bit(boards[color], sq);
  pop_bit(boards[piece], sq);
  set_bit(boards[color], target_sq);
  set_bit(boards[piece], target_sq);
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
  
  bb |= bb >> 1;
  bb |= bb >> 2;
  bb |= bb >> 4;
  bb |= bb >> 8;
  bb |= bb >> 16;
  bb |= bb >> 32;

  bb+=1;
  bb = bb >> 1;

  return bit_scan_forward(bb);

}

int get_opposite(int color) {
  if (color == white ) return black;
  else return white;
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

  int lsb = bit_scan_forward(bb);
  int msb = bit_scan_reverse(bb);

  printf("    lsb index: %d\n\n", lsb);
  printf("    msb index: %d\n\n", msb);

  }

U64 get_bb(int color, int piece) {
  return (boards[color] & boards[piece]);
}

U64 get_occupied() {
  return (boards[white] | boards[black]);
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

    for (int color = 0; color > 2; color++) {
      pawn_attacks[color][sq] = pawn_attack(sq, color);
    }

    horse_attacks[sq] = horse_attack(sq);
    king_attacks[sq] = king_attack(sq);


  }
}

void init_boards() {
  boards[white] = 0xffffULL;
  boards[black] = 0xffff000000000000ULL;
  boards[pawn] = 0x00ff00000000ff00ULL;

  set_bit(boards[horse], b1);
  set_bit(boards[horse], g1);
  set_bit(boards[horse], b8);
  set_bit(boards[horse], g8);

  set_bit(boards[king], d1);
  set_bit(boards[king], d8);

  set_bit(boards[queen], e1);
  set_bit(boards[queen], e8);

  set_bit(boards[rook], a1);
  set_bit(boards[rook], a1);
  set_bit(boards[rook], h1);
  set_bit(boards[rook], h8);

  set_bit(boards[bishop], c1);
  set_bit(boards[bishop], f1);
  set_bit(boards[bishop], c8);
  set_bit(boards[bishop], f8);


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

void get_moves(int color) {
  
  U64 occupied = get_occupied();
  U64 moves, temp;
  int sq, target_sq;
  int opposite = get_opposite(color);

  U64 pawns = get_bb(color, pawn);
  U64 horses = get_bb(color, horse);
  U64 kings = get_bb(color, king);
  U64 queens = get_bb(color, queen);
  U64 rooks = get_bb(color, rook);


  U64 bishops = get_bb(color, bishop);
  while (bishops) {
    sq = bit_scan_forward(bishops);
    printf("\n\nFOR BISHOP @%d:", sq);

    moves = get_bishop_attack(occupied, sq);
    moves &= ~boards[color];

    temp = moves & boards[opposite];
    moves &= ~boards[opposite];
    
    printf("\n   tactical moves:");
    while (temp) {
      target_sq = bit_scan_forward(temp);
      printf(" %d ", target_sq);
      pop_bit(temp, target_sq);
    }
    
    printf("\n   quiet moves:");
    while (moves) {
      target_sq = bit_scan_forward(moves);
      printf(" %d ", target_sq);
      pop_bit(moves, target_sq);
    }
    pop_bit(bishops, sq);
  }
  




}


/***********************************************\
 * =========================================== *
                 Main Driver
 * =========================================== *
\***********************************************/

int main()  {
    printf("\n\nhello\n\n");

    init_boards();
    init_attack();

    move_piece(white, pawn, d2, d4);
    move_piece(black, pawn, d7, d5);
    move_piece(white, bishop, c1, f4);

    printbb(get_occupied());
    get_moves(white);

    printf("\n\n\n");
    return 0;
}