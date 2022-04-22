#include "bitboards.h"
#include "position.h"
#include "generate.h"
#include "evaluation.h"
#include "make.h"
#include "timer.h" 

#ifndef SEARCH_H
#define SEARCH_H


static int is_repetition(position *pos);
static void checkup(search_info *info);
static int alphabeta(int alpha, int beta, int depth, position *pos, search_info *info);
int search(position *pos, search_info *info);
#endif
