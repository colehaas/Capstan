#include "search.h"

static int is_repetition(position *pos) {
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

static void checkup(search_info *info) {
    if (ns() > info->stop_time) {
        info->stopped = 1;
    }
}

static void clear_search(position *pos, search_info *info) {
    info->stopped = 0;
}

static int alphabeta(int alpha, int beta, int depth, position *pos, search_info *info) {
    
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
        

        score = -alphabeta(-beta, -alpha, depth-1, pos, info);
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

int search(position *pos, search_info *info) {
    int best_move = 0;
    int best_score = -INFINITE;
    int currrent_depth = 3;
    
    if (best_move == 0) {
        best_move = alphabeta(-INFINITE, INFINITE, currrent_depth, pos, info);
    }
    return best_move;
}