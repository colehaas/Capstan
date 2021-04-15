#include "evaluation.h"


//returns the piece balance value at a position
int value(position pos) {
    int value = 0;
    value += val_wp * pop_count(get_bb(pos.boards[white], pos.boards[pawn]));
    value += val_wn * pop_count(get_bb(pos.boards[white], pos.boards[horse]));
    value += val_wq * pop_count(get_bb(pos.boards[white], pos.boards[queen]));
    value += val_wr * pop_count(get_bb(pos.boards[white], pos.boards[rook]));
    value += val_wb * pop_count(get_bb(pos.boards[white], pos.boards[bishop]));
    value += val_bp * pop_count(get_bb(pos.boards[black], pos.boards[pawn]));
    value += val_bn * pop_count(get_bb(pos.boards[black], pos.boards[horse]));
    value += val_bq * pop_count(get_bb(pos.boards[black], pos.boards[queen]));
    value += val_br * pop_count(get_bb(pos.boards[black], pos.boards[rook]));
    value += val_bb * pop_count(get_bb(pos.boards[black], pos.boards[bishop]));
    return value;
}


int evalulation(position pos) {
    return value(pos);
}