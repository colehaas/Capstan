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

int isRepetition(position *pos) {
    //comparing current board position against all previous positions
    //can eventually add a simplification to include 50 move rule
    for (int i = 0; i < pos->index - 1; i++) { 
        //need a way to compare old board vs new board
        if (pos->hash_key == pos->undo[i].hash_key) {
            return 1;
        }
    }
    return 0;
}

int alphabeta(int alpha, int beta, int depth, position *pos) {
    
    //good place to add a node counter

    if (depth <= 0) return evaluation(*pos);

    //check for draw and depth

    move_list moves;
    moves = generate_moves(*pos);
    printmoves(moves);
    
    int movenum = 0;
    int legal = 0;
    int oldalpha = alpha;
    int best_move = 0;
    int score = -INFINITE;

    for (int m = 0; m < moves.count; m++) {

        //if move is illegal break/continue, else proceed
        if (!make(pos, moves.moves[m])) {
            unmake(pos);
            continue;
        }
        

        score = -alphabeta(-beta, -alpha, depth-1, pos);
        unmake(pos);

        if (score > alpha) {
            if (score >= beta) {
                return beta;
            }
            alpha = score;
            best_move = moves.moves[m];
        }
    }

    return best_move;

}

int search(position *pos) {
    int best_move = 0;
    int best_score = -INFINITE;
    int currrent_depth = 3;
    
    if (best_move == 0) {
        best_move = alphabeta(-INFINITE, INFINITE, currrent_depth, pos);
    }
    return best_move;
}