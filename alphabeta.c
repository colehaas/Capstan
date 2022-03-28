#include "alphabeta.h"

int searchmax(position *pos, int depth, int min, int max) {
    if (!depth) return evaluation(*pos);
    int x, y;
    //x is the starting beta value
    x = min;
    //generate all moves and loop through moves
    //probably shoudnt actually look through all of the moves at this time
    move_list moves = generate_moves(*pos);

    for (int m = 0; m < moves.count; m++) { 
        y = searchmin(moves.moves[m], depth - 1, x, max);
        if (y > x) x = y;
        if (x > max) return max;
    }

    return x;
}

int searchmin(position *pos, int depth, int min, int max) {
    if (!depth) return evaluation(*pos);
    int x, y;
    //x is the starting alpha value
    x = max;
    //generate all moves
    //probably shoudnt actually look through all of the moves at this time
    //
    move_list moves = generate_moves(*pos);


    for (int m = 0; m < moves.count; m++) { 
        y = searchmax(moves.moves[m], depth - 1, min, x);
        if (y < x) x = y;
        if (x < min) return min;
    }

    return x;
}