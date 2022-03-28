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
    parse(&pos, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    bb_to_square(&pos);

    gamelist game[256];
    //move_list moves;
    
    printpos(pos);
    printgb(pos);
    int val = value(pos);
    printf("\n\nvalue: %d", val);
    //play(&pos, game);

/*     printf("\n\nperft results:\n\n");
    for (int i = 1; i < 7; i++) {
        U64 result = perft(&pos, game, i);
        printf("%2d: %lld\n", i, result);
    } */
  
    printf("\n\n\n");
    return 0;
}