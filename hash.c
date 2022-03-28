#include "hash.h"

//one for each piececolor
U64 hash_values[13][64];
tpt transp[0x400000];

U64 xorshift(U64 x) {
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

void init_hash() {
    U64 seed = 0x12837127093;
    for (int x = 0; x < 13; x++) {
        for(int y = 0; y < 64; y++) {
            seed = xorshift(seed);
            hash_values[x][y] = seed; 
        }
    }
}


//hashing full board from square (probably best method right?)
//maybe its not but lets try it first
//input square board
//returns hash value

U64 hash_board(position pos) {
    U64 hash = 0x0;
    int color, piece, index; 
    
    //add all 64 squares to hasher
    for (int sq = 0; sq < 64; sq++) {
        color = get_square_color(pos, sq);
        piece = get_square_piece(pos, sq);
        index = (color * 6) + piece;
        hash |= hash_values[index][sq];     
    }
    
    //add 4 castling rights, 1 turn, and 8 enpassant files to hasher
    
    //get turn from pos argument
    unsigned short turn = pos.turn;
    if (turn) hash |= hash_values[12][0];

    //get castling rights from pos argument 
    unsigned short castle = pos.castle;
    if (decode_castle_wk(castle)) hash |= hash_values[12][1];
    if (decode_castle_wq(castle)) hash |= hash_values[12][2];
    if (decode_castle_bk(castle)) hash |= hash_values[12][3];
    if (decode_castle_bq(castle)) hash |= hash_values[12][4];

     //get enpassant from pos argument
     unsigned short enpassant = pos.enpassant;
     if (decode_enpassant_flag(enpassant)) {
        int offset = decode_enpassant_rank(enpassant);
        hash |= hash_values [12][5 + offset];
     }

    return hash;
}




//compare hash


//add to hash
//add square picece color
//add enpassant
//add castling rights(I guess maybe for going backwards)
//add 




//remove from hash
