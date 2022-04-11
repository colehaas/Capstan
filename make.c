/*****************************************************************************\
 * ========================================================================= *   
  +\^&%!%$/?^&\$!%^^&!<!%\+^\&%+\!!^`\?^%?<}%^&!^&!<!%\+^\^&!<!%\+^\?\?!%\+^
  %!%^`$&+"&^%^\`\^&!<!%\+^\"&^%              &!$^!>%!"$^%?<}%^&$^%?<}%^&\!%
  ^+\&+\^&!`\%$"&^%^^&!<!+$%^!      make.c      !^$\!!^^%?<^&!<!%\+^\}&+\^&!
  ^\?&%"&^%\"&^%^%&$^&!<!%\+^\^&              !<!^<&!+\!$%$!"$%\\!!^`\?!%\+^
  ^$%^$%`\]!^\!!^`\?!%\+^+\^&`$\$!"$!`\$!\$%\!!^`\?`\%$^&!<\?!%\+^&%?`\>!!%\
 * ========================================================================= *
\*****************************************************************************/
 
#include "make.h"

/*****************************************************************************\
                             HELPER FUNCTIONS
\*****************************************************************************/

void make_quiet(position *pos, int source, int target) {

  //get source piece
  int color = get_square_color(pos, source);
  int piece = get_square_piece(pos, source);

  //remove source piece from source sq 
  pop_bit(pos->boards[color], source);
  pop_bit(pos->boards[piece], source);
  remove_square(pos, source);

  //add source piece to target sq
  set_bit(pos->boards[color], target);
  set_bit(pos->boards[piece], target);
  add_square(pos, target, color, piece);
}

void make_capture(position *pos, int source, int target) {
  
  //get source piece
  int color = get_square_color(pos, source);
  int piece = get_square_piece(pos, source);

  //get capture piece and save in gamelist
  int capture = get_square_piece(pos, target);
  pos->undo[pos->index].captured = capture;

  //remove source piece from source sq
  pop_bit(pos->boards[color], source);
  pop_bit(pos->boards[piece], source);

  //remove target piece from target sq
  pop_bit(pos->boards[get_opposite(color)], target);
  pop_bit(pos->boards[capture], target);

  //add source piece to target sq 
  set_bit(pos->boards[color], target);
  set_bit(pos->boards[piece], target);

  move_square(pos, source, target);
}

void make_enpassant(position *pos, int source, int target) {
  int color = get_square_color(pos, source);
  int piece = get_square_piece(pos, source);
  //get enpassant sq
  int enpassant;
  if (color == white) {
    enpassant = get_sq(decode_enpassant_rank(pos->enpassant), 5);
  }
  else {
    enpassant = get_sq(decode_enpassant_rank(pos->enpassant), 4);
  }
  
  pos->undo[pos->index].captured = pawn;

  //remove source piece from source sq
  pop_bit(pos->boards[color], source);
  pop_bit(pos->boards[piece], source);

  //remove enpassant sq
  pop_bit(pos->boards[get_opposite(color)], enpassant);
  pop_bit(pos->boards[pawn], enpassant);
  remove_square(pos, enpassant);

  //move source piece to target
  set_bit(pos->boards[color], target);
  set_bit(pos->boards[piece], target);

  move_square(pos, source, target);
  
}

void make_quiet_promotion(position *pos, int source, int target, int promotion) {
  int color = get_square_color(pos, source);
  int piece = get_square_piece(pos, source);
  pop_bit(pos->boards[color], source);
  pop_bit(pos->boards[piece], source);
  set_bit(pos->boards[color], target);
  set_bit(pos->boards[promotion], target);
  promotion_square(pos, source, target, promotion);
}

void make_capture_promotion(position *pos, int source, int target, int promotion) {  
  int color = get_square_color(pos, source);
  int piece = get_square_piece(pos, source);
  int capture = get_square_piece(pos, target);
  pos->undo[pos->index].captured = capture;
  //remove source
  pop_bit(pos->boards[color], source);
  pop_bit(pos->boards[piece], source);
  //remove target
  pop_bit(pos->boards[get_opposite(color)], target);
  pop_bit(pos->boards[capture], target);
  //add source to target
  set_bit(pos->boards[color], target);
  set_bit(pos->boards[promotion], target);
  promotion_square(pos, source, target, promotion);
} 

//this group of functions return 1 if illegal, else 0

int make_wk_castle(position *pos) {
  //king e1->g1
  make_quiet(pos, e1, g1);
  //rook h1->f1
  make_quiet(pos, h1, f1);
  //return if move is legal
  return (sq_attacked(*pos, e1)||sq_attacked(*pos, g1)||sq_attacked(*pos, f1));
}

int make_bk_castle(position *pos) {
  //king e8->g8
  make_quiet(pos, e8, g8);
  //rook h8->f8
  make_quiet(pos, h8, f8);
  //return if move is legal
  return (sq_attacked(*pos, e8)||sq_attacked(*pos, g8)||sq_attacked(*pos, f8));
}

int make_wq_castle(position *pos) {
  //king e1->c1
  make_quiet(pos, e1, c1);
  //rook a1->d1
  make_quiet(pos, a1, d1);
  //return if move is legal
  return (sq_attacked(*pos, c1)||sq_attacked(*pos, d1)||sq_attacked(*pos, e1));
}

int make_bq_castle(position *pos) {
  //king e8->c8
  make_quiet(pos, e8, c8);
  //rook a8->d8
  make_quiet(pos, a8, d8);
  //return if move is legal
  return (sq_attacked(*pos, c8)||sq_attacked(*pos, d8)||sq_attacked(*pos, e8));
}


void unmake_capture(position *pos, int source, int target) {

  //current state: source piece on target sq, capture piece absent

  //get source piece from target sq
  int color = get_square_color(pos, target);
  int piece = get_square_piece(pos, target);

  //get capture piece from memory
  int capture = pos->undo[pos->index].captured;

  //remove source piece off target sq
  pop_bit(pos->boards[color], target);
  pop_bit(pos->boards[piece], target);

  //add capture piece to target sq
  set_bit(pos->boards[get_opposite(color)], target);
  set_bit(pos->boards[capture], target);
  add_square(pos, target, get_opposite(color), capture);

  //add source piece to source sq
  set_bit(pos->boards[color], source);
  set_bit(pos->boards[piece], source);
  add_square(pos, source, color, piece);

}

void unmake_enpassant(position *pos, int source, int target) {
  int color = get_square_color(pos, target);
  int piece = get_square_piece(pos, target);
  //get enpassant sq
  int enpassant;
  if (color == white) {
    enpassant = get_sq(decode_enpassant_rank(pos->undo[pos->index].enpassant), 5);
  }
  else {
    enpassant = get_sq(decode_enpassant_rank(pos->undo[pos->index].enpassant), 4);
  }

  //remove source piece from target sq
  pop_bit(pos->boards[color], target);
  pop_bit(pos->boards[piece], target);

  //add target piece to enpassant sq
  set_bit(pos->boards[get_opposite(color)], enpassant);
  set_bit(pos->boards[pawn], enpassant);
  add_square(pos, enpassant, get_opposite(color), pawn);

  //add source piece to source square
  set_bit(pos->boards[color], source);
  set_bit(pos->boards[piece], source);
  
  move_square(pos, target, source);
  
}

void unmake_quiet_promotion(position *pos, int source, int target) {
  
  //current state: source piece gone(pawn), promotion piece on target sq
  
  //get source color and promotion piece from target sq
  int color = get_square_color(pos, target);
  int piece = get_square_piece(pos, target);

  //remove promotion piece off target sq
  pop_bit(pos->boards[color], target);
  pop_bit(pos->boards[piece], target);
  remove_square(pos, target);

  //add pawn to source sq
  set_bit(pos->boards[color], source);
  set_bit(pos->boards[pawn], source);
  add_square(pos, source, color, pawn);
  
}

void unmake_capture_promotion(position *pos, int source, int target) {

  //current state: source piece gone(pawn), target piece gone
  //promotion piece on target sq

  //get source color
  int color = get_square_color(pos, target);
  //get promtion piece
  int piece = get_square_piece(pos, target);
  //get capture piece from memory
  int capture = pos->undo[pos->index].captured;

  //remove promotion piece off target sq
  pop_bit(pos->boards[color], target);
  pop_bit(pos->boards[piece], target);

  //add captured piece to target sq
  set_bit(pos->boards[get_opposite(color)], target);
  set_bit(pos->boards[capture], target);
  add_square(pos, target, get_opposite(color), capture);

  //add pawn to source sq
  set_bit(pos->boards[color], source);
  set_bit(pos->boards[pawn], source);
  add_square(pos, source, color, pawn);

  
}


void unmake_wk_castle(position *pos) {
  //king g1->e1
  make_quiet(pos, g1, e1);
  //rook f1->h1
  make_quiet(pos, f1, h1);

}

void unmake_bk_castle(position *pos) {
  //king g8->e8
  make_quiet(pos, g8, e8);
  //rook f8->h8
  make_quiet(pos, f8, h8);
}

void unmake_wq_castle(position *pos) {
  //king c1->e1
  make_quiet(pos, c1, e1);
  //rook d1->a1
  make_quiet(pos, d1, a1);
}

void unmake_bq_castle(position *pos) {
  //king c8->e8
  make_quiet(pos, c8, e8);
  //rook d8->a8
  make_quiet(pos, d8, a8);
}



/*****************************************************************************\
                                   MAKE
\*****************************************************************************/

//returns 1 if illegal, else 0
//index should start pointing at the move being made
int make(position *pos, int move) {
  
  //0 if legal, 1 if illegal
  int illegal = 0;

  //get source, target squares
  int source = decode_source(move);
  int target = decode_target(move);

  //get move flags to determine move type
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);
  
  //remember move
  //get captured later
  pos->undo[pos->index].move = move;
  pos->undo[pos->index].enpassant = pos->enpassant;
  pos->undo[pos->index].castle = pos->castle;

  pos->undo[pos->index].hash_key = pos->hash_key;

  //keep track of enpassant through move
  int enpassant = 0;



  //change castling rights
  //if rook or king move
  int piece = get_square_piece(pos, source);
  if ((piece == king)||(piece == rook)) {
    int wk = decode_castle_wk(pos->castle);
    int bk = decode_castle_bk(pos->castle);
    int wq = decode_castle_wq(pos->castle);
    int bq = decode_castle_bq(pos->castle);
    //king
    if (piece == king) {
      if (!pos->turn) {
        wk = 0;
        wq = 0;
      }
      else {
        bk = 0;
        bq = 0;
      }
    }
    //rooks
    else {
      //white
      if (!pos->turn) {
        if (source == a1) {
          wq = 0;
        }
        else if (source == h1) {
          wk = 0;
        }
      }
      else {
        if (source == a8) {
          bq = 0;
        }
        else if (source == h8) {
          bk = 0;
        }
      }
    }
    pos->castle = encode_castle(wk, wq, bk, bq);
  }
  //rook is captured
  piece = get_square_piece(pos, target);
  //printf("piece = %d", piece);
  if (piece == rook) {
    int wk = decode_castle_wk(pos->castle);
    int bk = decode_castle_bk(pos->castle);
    int wq = decode_castle_wq(pos->castle);
    int bq = decode_castle_bq(pos->castle);
    //if blacks turn white rook captured
    if (pos->turn) {
      if (target == a1) {
        wq = 0;
      }
      else if (target == h1) {
        wk = 0;
      }
    }
    else {
      if (target == a8) {
        bq = 0;
      }
      else if (target == h8) {
        bk = 0;
      }
    }
    pos->castle = encode_castle(wk, wq, bk, bq);
  }



  //make move
  if (!promotion) {
    if (!capture) {
      //quiet and double push 000*
      if (!y) {
        make_quiet(pos, source, target);
        //double push 0001
        if (z) {
          //target rank = target % 8
          enpassant = encode_enpassant(1, target % 8);
        }
      }
      //castle 001*
      else {
          //get current castle flags
          int wk = decode_castle_wk(pos->castle);
          int bk = decode_castle_bk(pos->castle);
          int wq = decode_castle_wq(pos->castle);
          int bq = decode_castle_bq(pos->castle);
        //king castle 0011
        if (z) {
          //white
          if (!pos->turn) {
            //king e1->g1, rook h1->f1
            illegal = make_wk_castle(pos);
            wk = 0;
            wq = 0;
            pos->castle = encode_castle(wk, wq, bk, bq);
          }
          //black
          else {
            //king e8->g8, rook h8->f8
            illegal = make_bk_castle(pos);
            bk = 0;
            bq = 0;
            pos->castle = encode_castle(wk, wq, bk, bq);
          }
        }
        //queen castle 0010
        else {
          //white
          if (!pos->turn) {
            //king e1->c1, rook a1->d1
            illegal = make_wq_castle(pos);
            wk = 0;
            wq = 0;
            pos->castle = encode_castle(wk, wq, bk, bq);
          }
          //black
          else {
            //king e8->c8, rook a8->d8
            illegal = make_bq_castle(pos);
            bk = 0;
            bq = 0;
            pos->castle = encode_castle(wk, wq, bk, bq);
          }
        }
      }
    }
    //capture 01**
    else {
      //enpassant 0101
      if (z) {
        //printf("ENPASSANT");
        pos->undo[pos->index].captured = pawn;
        make_enpassant(pos, source, target);
      }
      //capture 01*0 
      else {
        pos->undo[pos->index].captured = get_piece_sq(*pos, target);
        make_capture(pos, source, target);
      }
    }
  }
  //promotions
  else {
    int promotion = promotion_piece(y, z);
    //quiet promotion 10**
    if (!capture) {
      make_quiet_promotion(pos, source, target, promotion);
    }
    //capture promotion 11**
    else {
      pos->undo[pos->index].captured = get_piece_sq(*pos, target);
      make_capture_promotion(pos, source, target, promotion);
    }
  }

  //check if move is illegal, leaves king in check (piece is pinned), or castling not available
  U64 king_bb = get_bb(pos->boards[pos->turn], pos->boards[king]);
  int king_sq = bit_scan_forward(king_bb);
  if (sq_attacked(*pos, king_sq)) {
    illegal = 1;
  }
  

  //set hash
  pos->hash_key = hash_board(pos);
  //switch turn, set new enpassant, and increment index
  pos->turn = get_opposite(pos->turn);
  pos->enpassant = enpassant;
  pos->index++;

  return illegal;
}

/*****************************************************************************\
                                  UNMAKE
\*****************************************************************************/

//index should start poiting at the next move
void unmake(position *pos) {

  //erase current move
  pos->undo[pos->index].move = 0;
  pos->undo[pos->index].castle = 0;
  pos->undo[pos->index].enpassant = 0;
  pos->undo[pos->index].captured = 0;

  //move to correct move and get move
  pos->index--;
  int move = pos->undo[pos->index].move;

  //get irreversible data
  pos->castle = pos->undo[pos->index].castle;
  pos->enpassant = pos->undo[pos->index].enpassant;

  //switch back turn
  pos->turn = get_opposite(pos->turn);
  
  //decode source and target sq
  int source = decode_source(move);
  int target = decode_target(move);

  //decode move flags
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);

  //0***
  if (!promotion) {
    //00**
    if (!capture) {
      //000* quiet and double push 
      if (!y) {
        //reverse target and source to unmake quiet
        make_quiet(pos, target, source);
        //no need to worry about double push
      }
      //001* castle 
      else {
        //king castle 0011
        if (z) {
          //white
          if (!pos->turn) {
            //king e1->g1, rook h1->f1
            unmake_wk_castle(pos);
          }
          //black
          else {
            //king e8->g8, rook h8->f8
            unmake_bk_castle(pos);
          }
        }
        //queen castle 0010
        else {
          //white
          if (!pos->turn) {
            //king e1->c1, rook a1->d1
            unmake_wq_castle(pos);
          }
          //black
          else {
            //king e8->c8, rook a8->d8
            unmake_bq_castle(pos);
          }
        }
      }
    }
    //captures 01**
    else {
      //enpassant 0101
      if (z) {
        unmake_enpassant(pos, source, target);
      }
      //capture 0100
      else {
        unmake_capture(pos, source, target);
      }
    }
  }
  //promotions
  else {
    //quiet promotion 10**
    if (!capture) {
      unmake_quiet_promotion(pos, source, target);
    }
    //capture promotion 11**
    else {
      unmake_capture_promotion(pos, source, target);
    }
  }  
}


