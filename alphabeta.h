#include "bitboards.h"
#include "position.h"
#include "generate.h"
#include "evaluation.h"

#ifndef ALPHABETA_H
#define ALPHABETA_H

int searchmax(position *pos, int depth, int min, int max);
int searchmin(position *pos, int depth, int min, int max);

#endif
