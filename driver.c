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
#include "evaluation.h"
#include "timer.h"

#include "search.h"


int main() {
    
    printf("\n\nhello\n\n");
    uint64_t start = ns();
    perft_test("perft.txt", 20, 3);
    uint64_t stop = ns();
    printf("time: %lld", stop - start);
    position pos;

    //initalize attack arrays
    init_attack();
    //initialize position struct
    init_position(&pos);

    //get position from fen
    //parse(&pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

    parse(&pos, "r4B2/p1nr1kp1/1pN1pn1p/3p1p2/2PP4/1PN3PP/P4PK1/2RR4 b - - 0 24");
    bb_to_square(&pos);

    undo_list game[256];
    //move_list moves;
    
    printpos(pos);
    printgb(pos);
    int val = value(pos);
    printf("\n\nvalue: %d", val);
    //play(&pos, game);

    alphabeta(-30000,30000,3,&pos);
/*
    printf("\n\nperft results:\n\n");
    for (int i = 1; i < 6; i++) {
        U64 result = perft(&pos, game, i);
        printf("%2d: %lld\n", i, result);
    }
*/  
    printf("\n\n\n");
    return 0;
}