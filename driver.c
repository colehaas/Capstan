/***********************************************\
 * =========================================== *
                 Main Driver
 * =========================================== *
\***********************************************/


#include "bitboards.h"
#include "attack.h"
#include "position.h"
#include "generate.h"
#include "make.h"
#include "square.h"

int main() {
    
    printf("\n\nhello\n\n");

    position position;

    init_position(&position);
    init_attack();
    printsquareboard(position);

/* 
    move_piece(&position, white, pawn, d2, d4);
    move_piece(&position, black, pawn, d7, d5);
    move_piece(&position, white, bishop, c1, f4);
    move_piece(&position, black, pawn, c7, c4);
    move_piece(&position, white, pawn, h2, h4);
    move_piece(&position, black, pawn, a7, b6);
    move_piece(&position, white, pawn, a2, a7);
*/

    position = parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    printf("\n\n%d%d\n\n", decode_enpassant_flag(position.enpassant), decode_enpassant_rank(position.enpassant));
    //play(position);
    bb_to_square(&position);
    
    U64 nodes = perft(position, 1);
    printf("\n\n%lld", nodes);
    


/*
    //printbb(get_occupied(position.boards[white], position.boards[black]));
    printgb(position);
    
    move_list moves = generate_moves(position, white);
    printmoves(moves);
    
*/
  
    printf("\n\n\n");
    return 0;
}