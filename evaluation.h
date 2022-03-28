#ifndef EVALUATION_H
#define EVALUATION_H

#include "bitboards.h"
#include "position.h"

enum {
    val_wp = 100,
    val_wn = 300,
    val_wb = 350,
    val_wr = 500,
    val_wq = 1000,
    val_bp = -100,
    val_bn = -300,
    val_bb = -350,
    val_br = -500,
    val_bq = -1000
};

int value(position pos);
int evaluation(position pos);

#endif