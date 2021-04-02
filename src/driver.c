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

    int illegal;
    position position;
    init_position(&position);
    init_attack();
    position = parse_fen("2rb4/r7/6k1/1p6/3p1KP1/R1N2NB1/1p6/8 w      ");
    bb_to_square(&position);
    printgb(position);

    for (int i = 0; i < 3; i++) {
        move_list moves = generate_moves(position);
        printmoves(moves);
        position = make(position, moves.moves[4], &illegal);
        printgb(position);
    }
    unmake(position);
    printgb(position);


    
    U64 nodes = perft(position, 1);
    printf("\n\n%lld", nodes);
  
    printf("\n\n\n");
    return 0;
}