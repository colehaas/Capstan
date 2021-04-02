/*****************************************************************************\
 * ========================================================================= *    
  &^%^%%^%?!/|!^&)!?<$%^$$/?^!%^&\`$^&!\%+\$!%^\?^<?%}$\%$^&>!$%^<>?^&+\^%^&
  ?^&\$`%$%>"&^%^%^$!%+\^&%!%$/?^&+\^&%!\%$/?^&\$&\%$/?^!%^&%+\$!%^\?&%+\!^?
  ^%&`\}/\`$^&!\%$/\&+$+?\$%%^                 +$%^`%+$%^&!+?\$%%^!%\+$<%^&$
  \`$%\`?^%?<}%^&$%^>?%^&       C A P S T A N       `\%$`\$%$>^<?%}$%$^%%^!%
  !\$/\`%`\%$^&!<!%^&>    a  c h e s s  e n g i n e     $%+\`?\$%^!%\+$%^`%!
  &%^$[$!\?+\%^$\!^%^$                                  !%$!/?^&+\^&!+?\$%!?
  \$%^+%$%>"&^%^\$^`$$^&          by 16haas           &!^!`&<&>!`%\\$!\'$\`$
  \%$`/&^!}?!%}|%>!<"~|}!$\                        $^%/%&^%$`\%$!!%^"$%\`$`%
  `%$^?\$^\&^$`%?\%$%>"&^%^%%^%?!/|_^&)!?<$%^$&^<&>!`%\$!\!`%^$%^$%`\]!!$\`/
  %>"&^%^%^$!%+\^&%!%&!\%$%\?+\%$%\?+\%$^!$/$$/?^!%^&\`$^&!\%`\%$^&>!+\$!%^&
 * ========================================================================= *
\******************************************************************************/

/*************************************\
 * ================================= *
  +\^&%!%$/?^&\$!%^&%+\!!^`\?!%\+^\?  
  %!%^`$!"&!$          ^!>%!"$%!%\!%
  ^+\$%^!$!    make.c   ?|&!^$&%?`\>
  ^\?&%+\!$%           `$!"$%\!%$!%$
  ^$%^$%`\]!^+\^&`$!"$!`\$!\$%!%\^<!
 * ================================= *
\*************************************/
 
#include "make.h"

//for make function
int get_rank(int sq) {
  return (sq % 8);
}

/***************************\
       MAKE & UNMAKE   
\***************************/

void make_quiet(position *position, int source, int target, int color, int piece) {
  pop_bit(position->boards[color], source);
  pop_bit(position->boards[piece], source);
  set_bit(position->boards[color], target);
  set_bit(position->boards[piece], target);
  move_square(*position, source, target);
}

void make_capture(position *position, int source, int target, int color, int piece, int capture) {
  pop_bit(position->boards[color], source);
  pop_bit(position->boards[piece], source);
  pop_bit(position->boards[get_opposite(color)], target);
  pop_bit(position->boards[capture], target);
  set_bit(position->boards[color], target);
  set_bit(position->boards[piece], target);
  move_square(*position, source, target);
}

void make_quiet_promotion(position *position, int source, int target, int color, int piece, int promotion) {
  pop_bit(position->boards[color], source);
  pop_bit(position->boards[piece], source);
  set_bit(position->boards[color], target);
  set_bit(position->boards[promotion], target);
  promotion_square(*position, source, target, promotion);
}

void make_capture_promotion(position *position, int source, int target, int color, int piece, int capture, int promotion) {  
  pop_bit(position->boards[color], source);
  pop_bit(position->boards[piece], source);
  pop_bit(position->boards[get_opposite(color)], target);
  pop_bit(position->boards[capture], target);
  set_bit(position->boards[color], target);
  set_bit(position->boards[promotion], target);
  promotion_square(*position, source, target, promotion);
} 


position make(position from, int move, int *illegal) {
  
  position to = from;
  int source = decode_source(move);
  int target = decode_target(move);
  int piece = decode_piece(move);
  int color = decode_color(move);
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);
  int capture_piece;


  to.game_list.moves[to.game_list.count++] = move;

  if (promotion==0) {
    if (capture==0) {
      //quiet and double push 000*
      if (y==0) {
        make_quiet(&to, source, target, color, piece);
        //double push 0001
        if (z==1) to.enpassant = encode_enpassant(1, get_rank(target));
      }
      //castle 001*
      else {
        //king castle 0011
        if (z==1) {
          //white
          if (color == white) {
            //king e1->g1, rook h1->f1
            make_quiet(&to, e1, g1, white, king);
            make_quiet(&to, h1, f1, white, rook);
            //illegal move
            if (sq_attacked(to, e1)||sq_attacked(to, g1)||sq_attacked(to, f1)) {
              *illegal = 1;
              return to;
            }

          }
          //black
          else {
            //king e8->g8, rook h8->f8
            make_quiet(&to, e8, g8, black, king);
            make_quiet(&to, h8, f8, black, rook);
            //illegal move
            if (sq_attacked(to, e8)||sq_attacked(to, g8)||sq_attacked(to, f8)) {
              *illegal = 1;
              return to;
            }
          }
        }
        //queen castle 0010
        else {
          //white
          if (color == white) {
            //king e1->c1, rook a1->d1
            make_quiet(&to, e1, c1, white, king);
            make_quiet(&to, a1, d1, white, rook);
            //illegal move
            if (sq_attacked(to, c1)||sq_attacked(to, d1)||sq_attacked(to, e1)) {
              *illegal = 1;
              return to;
            }
          }
          //black
          else {
            //king e8->c8, rook a8->d8
            make_quiet(&to, e8, c8, black, king);
            make_quiet(&to, a8, d8, black, rook);
            //illegal move
            if (sq_attacked(to, c8)||sq_attacked(to, d8)||sq_attacked(to, e8)) {
              *illegal = 1;
              return to;
            }
          }
        }
      }
    }
    //capture 01**
    else {
      capture_piece = get_piece_sq(from, target);
      make_capture(&to, source, target, color, piece, capture_piece);
    }
  }
  //promotions
  else {
    int promotion = promotion_piece(y, z);
    //quiet promotion 10**
    if (capture == 0) {
      make_quiet_promotion(&to, source, target, color, piece, promotion);
    }
    //capture promotion 11**
    else {
      make_capture_promotion(&to, source, target, color, piece, capture, promotion);
    }
  }

  //Illegal moves
  //check if move leaves king in check
  U64 king_bb = get_bb(to.boards[color], to.boards[king]);
  int king_sq = bit_scan_forward(king_bb);
  if (sq_attacked(to, king_sq)) {
    //move ilegal
    *illegal = 1;
    return to;
  }

  to.turn = get_opposite(from.turn);
  return to;
}

void unmake(position to) {

  //get previous move and erase
  int move = to.game_list.moves[--to.game_list.count];
  to.game_list.moves[to.game_list.count] = 0;

  //decode move variables
  int source = decode_source(move);
  int target = decode_target(move);
  int piece = decode_piece(move);
  int color = decode_color(move);
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);
  int capture_piece;

  //how to save irreversible data

  if (promotion==0) {
    if (capture==0) {
      //quiet and double push 000*
      if (y==0) {
        make_quiet(&to, target, source, color, piece);
        //double push 0001
        //if (z==1) to.enpassant = encode_enpassant(1, get_rank(target));
      }
      //castle 001*
      else {
        //king castle 0011
        if (z==1) {
          //white
          if (color == white) {
            //king e1->g1, rook h1->f1
            make_quiet(&to, g1, e1, white, king);
            make_quiet(&to, f1, h1, white, rook);
          }
          //black
          else {
            //king e8->g8, rook h8->f8
            make_quiet(&to, g8, e8, black, king);
            make_quiet(&to, f8, h8, black, rook);
          }
        }
        //queen castle 0010
        else {
          //white
          if (color == white) {
            //king e1->c1, rook a1->d1
            make_quiet(&to, c1, e1, white, king);
            make_quiet(&to, d1, a1, white, rook);
          }
          //black
          else {
            //king e8->c8, rook a8->d8
            make_quiet(&to, c8, e8, black, king);
            make_quiet(&to, d8, a8, black, rook);
          }
        }
      }
    }
    //capture 01**
    else {
      capture_piece = get_piece_sq(to, target);
      make_capture(&to, target, source, color, capture_piece, piece);
    }
  }
  //promotions
  else {
    int promotion = promotion_piece(y, z);
    //quiet promotion 10**
    if (capture == 0) {
      make_quiet_promotion(&to, target, source, color, promotion, piece);
    }
    //capture promotion 11**
    else {
      //wrong
      make_capture_promotion(&to, target, source, color, promotion, capture, piece);
    }
  }

  to.turn = get_opposite(to.turn);
  return to;
  
}

U64 perft(position start, int depth) {
  
  U64 nodes = 0x0ULL;
  position to = start;
  move_list moves;
  int illegal = 0;

  if (depth == 0) return 0x1ULL;

  moves = generate_moves(start);

  for (int c = 0; c < moves.count; c++) {
    to = make(start, moves.moves[c], &illegal);
    //if illegal move
    if (!illegal) {
      nodes += perft(to, depth - 1);
    }
  }
  return nodes;
}

void play(position position) {
  int move = 0;
  int color = position.turn;
  move_list moves;
  int illegal = 0;
  while (move != -1) {

    //print board and moves
    printf("\n\n");
    printcolor(color);
    printf("To Move:\n\n");
    printgb(position);
    moves = generate_moves(position);
    printmoves(moves);

    //get move from command line
    printf("\n\nEnter Move Number: ");
    scanf("%d", &move);


    if (move != -1) {
      //make move
      position = make(position, moves.moves[move], &illegal);
      //switch color
      color = get_opposite(color);
    }

  }
}



