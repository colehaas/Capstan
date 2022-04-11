#include "bitboards.h"
#include "position.h"
#include "generate.h"
#include "evaluation.h"
#include "make.h"

#ifndef SEARCH_H
#define SEARCH_H

int searchmax(position *pos, int depth, int min, int max);
int searchmin(position *pos, int depth, int min, int max);
int alphabeta(int alpha, int beta, int depth, position *pos);
int search(position *pos);
#endif
