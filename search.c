#include "search.h"

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

int alphabeta(int alpha, int beta, int depth, position *pos) {
    
    //good place to add a node counter

    if (depth <= 0) return evaluation(*pos);

    //check for draw and depth

    move_list moves;
    moves = generate_moves(*pos);
    
    int movenum = 0;
    int legal = 0;
    int oldalpha = alpha;
    int bestmove = 0;
    int score = -100000;

    for (int m = 0; m < moves.count; m++) {

        //an additional place where we could check legal moves

        score = -alphabeta(-beta, -alpha, depth-1, moves.moves[m]);

        if (score > alpha) {
            if (score >= beta) {
                return beta;
            }
            alpha = score;
            bestmove = moves.moves[m];
        }
    }

    return 0;

}

int search() {
    return 0;
}