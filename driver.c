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
#include "perft.h"



int main() {
    
    printf("\n\nhello\n\n");
    //perft_test("perft.txt", 100, 4);
    position pos;
    init_attack();

    for (int c = 0; c < 8; c++) pos.boards[c] = 0x0ULL;
    for (int sq = 0; sq < 64; sq++) pos.square[sq] = 0;
    pos.turn = white;
    pos.castle = encode_castle(1, 1, 1, 1);
    pos.enpassant = encode_enpassant(0, 0);
    pos.index = 0;

    parse(&pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    bb_to_square(&pos);

    gamelist game[256];
    //move_list moves;
    
    printpos(pos);
    printgb(pos);
    //play(&pos, game);

    printf("\n\nperft results:\n\n");
    for (int i = 1; i < 7; i++) {
        U64 result = perft(&pos, game, i);
        printf("%2d: %lld\n", i, result);
    }


/*     int i = 20;
    int illegal = 0;
    while(i) {
        moves = generate_moves(pos);
        printmoves(moves);
        illegal = make(&pos, game, moves.moves[6]);
        printmove(moves.moves[6]);
        if (illegal) {
            unmake(&pos, game);
        }
        printgb(pos);
        i--;
    }
    i = 20;
    while (i) {
        unmake(&pos, game);
        printgb(pos);
        i--;
    } */


    

    

  
    printf("\n\n\n");
    return 0;
}