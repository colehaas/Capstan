/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$         ^!>%!"$%!%\!%
  ^+\$%^!$!   chess.c   ?|&!^$&%?`\>
  ^\?&%+\!$%!         ^`$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/

#ifndef MAKE_H
#define MAKE_H

#include "position.h"
#include "square.h"
#include "bitboards.h"
#include "generate.h"


/*
castle, enpassant, captured are irreversible
and held onto for unmake function

MAYBE: eventually merge into position
*/

typedef struct {
  //move
  unsigned int move;
  //which castles can be done
  unsigned short castle;
  //current enpassantable pawns
  unsigned short enpassant;
  //piece captured
  unsigned short captured;
} gamelist;



void init_gamelist(gamelist *gamelist);

/***************************\
       MAKE & UNMAKE   
\***************************/

void make_quiet(position *pos, int source, int target);
void make_capture(position *pos, gamelist *gl, int source, int target);
void make_quiet_promotion(position *pos, int source, int target, int promotion);
void make_capture_promotion(position *pos, gamelist *gl, int source, int target, int promotion);

int make_wk_castle(position *pos);
int make_bk_castle(position *pos);
int make_wq_castle(position *pos);
int make_bq_castle(position *pos);

void unmake_capture(position *pos, gamelist *gl, int source, int target);
void unmake_quiet_promotion(position *pos, int source, int target);
void unmake_capture_promotion(position *pos, gamelist *gl, int source, int target);

void unmake_wk_castle(position *pos);
void unmake_bk_castle(position *pos);
void unmake_wq_castle(position *pos);
void unmake_bq_castle(position *pos);


int make(position *pos, gamelist *gl, int move);

void unmake(position *pos, gamelist *gl);
U64 perft(position *pos, gamelist *gl, int depth);


#endif