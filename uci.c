
#include "bitboards.h"
#include "attack.h"
#include "position.h"
#include "generate.h"
#include "make.h"
#include "square.h"
#include "perft.h"
#include "evaluation.h"
#include "timer.h"

//communication layer to interface
//UCI (Universal Chess Interface)



//GUI to engine



//engine to GUI

int parse_move(position pos, char * move_string) {
   
    //source and target 
    int source = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int target = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

    move_list moves = generate_moves(pos);
    


}


