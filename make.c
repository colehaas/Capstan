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

position make(position from, int move) {
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


  if (promotion==0) {
    if (capture==0) {
      //quiet and double push 000*
      if (y==0) {
        pop_bit(to.boards[color], source);
        pop_bit(to.boards[piece], source);
        set_bit(to.boards[color], target);
        set_bit(to.boards[piece], target);
        move_square(to, source, target);


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
            pop_bit(to.boards[king], e1);
            pop_bit(to.boards[white], e1);
            set_bit(to.boards[king], g1);
            set_bit(to.boards[white], g1);
            move_square(to, e1, g1);

            pop_bit(to.boards[rook], h1);
            pop_bit(to.boards[white], h1);
            set_bit(to.boards[rook], f1);
            set_bit(to.boards[white], f1);
            move_square(to, h1, f1);

            //illegal move
            if (sq_attacked(to, g1)||sq_attacked(to, f1)) {
              printf("ILLEGAL MOVE");
              empty_position(&to);
              return to;
            }

          }
          //black
          else {
            //king e8->g8, rook h8->f8
            pop_bit(to.boards[king], e8);
            pop_bit(to.boards[black], e8);
            set_bit(to.boards[king], g8);
            set_bit(to.boards[black], g8);
            move_square(to, e8, g8);


            pop_bit(to.boards[rook], h8);
            pop_bit(to.boards[black], h8);
            set_bit(to.boards[rook], f8);
            set_bit(to.boards[black], f8);
            move_square(to, h8, f8);
            
            //illegal move
            if (sq_attacked(to, g8)||sq_attacked(to, f8)) {
              printf("ILLEGAL MOVE");
              empty_position(&to);
              return to;
            }
          }
        }
        //queen castle 0010
        else {
          //white
          if (color == white) {
            //king e1->c1, rook a1->d1
            pop_bit(to.boards[king], e1);
            pop_bit(to.boards[white], e1);
            set_bit(to.boards[king], c1);
            set_bit(to.boards[white], c1);
            move_square(to, e1, c1);


            pop_bit(to.boards[rook], a1);
            pop_bit(to.boards[white], a1);
            set_bit(to.boards[rook], d1);
            set_bit(to.boards[white], d1);
            move_square(to, a1, d1);

            //illegal move
            if (sq_attacked(to, c1)||sq_attacked(to, d1)) {
              printf("ILLEGAL MOVE");
              empty_position(&to);
              return to;
            }

          }
          //black
          else {
            //king e8->c8, rook a8->d8
            pop_bit(to.boards[king], e8);
            pop_bit(to.boards[black], e8);
            set_bit(to.boards[king], c8);
            set_bit(to.boards[black], c8);
            move_square(to, e8, c8);

            pop_bit(to.boards[rook], a8);
            pop_bit(to.boards[black], a8);
            set_bit(to.boards[rook], d8);
            set_bit(to.boards[black], d8);
            move_square(to, a8, d8);
            
            //illegal move
            if (sq_attacked(to, c8)||sq_attacked(to, d8)) {
              printf("ILLEGAL MOVE");
              empty_position(&to);
              return to;
            }

          }
        }
      }
    }
    //capture 01**
    else {
      capture_piece = get_piece_sq(from, target);
      pop_bit(to.boards[color], source);
      pop_bit(to.boards[piece], source);

      pop_bit(to.boards[get_opposite(color)], target);
      pop_bit(to.boards[capture_piece], target);

      set_bit(to.boards[color], target);
      set_bit(to.boards[piece], target);

      move_square(to, source, target);
    }
  }
  //promotions
  else {
    int promotion = promotion_piece(y, z);
    //quiet promotion 10**
    if (capture == 0) {
      pop_bit(to.boards[color], source);
      pop_bit(to.boards[piece], source);
      set_bit(to.boards[color], target);
      set_bit(to.boards[promotion], target);
      promotion_square(to, source, target, promotion);
    }
    //capture promotion 11**
    else {
      capture_piece = get_piece_sq(from, target);
      pop_bit(to.boards[color], source);
      pop_bit(to.boards[piece], source);

      pop_bit(to.boards[get_opposite(color)], target);
      pop_bit(to.boards[capture_piece], target);

      set_bit(to.boards[color], target);
      set_bit(to.boards[promotion], target);
      promotion_square(to, source, target, promotion);
    }
  }
  //Illegal moves
  //check if move leaves king in check
  U64 king_bb = get_bb(to.boards[color], to.boards[king]);
  int king_sq = bit_scan_forward(king_bb);
  if (sq_attacked(to, king_sq)) {
    //move ilegal
    //best way to remove move
    printf("ILLEGAL MOVE");
    empty_position(&to);
    return to;
  }

  to.turn = get_opposite(from.turn);
  return to;
}


U64 perft(position start, int depth) {
  
  U64 nodes = 0x0ULL;
  position to = start;
  move_list moves;

  if (depth == 0) return 0x1ULL;

  moves = generate_moves(start);

  for (int c = 0; c < moves.count; c++) {
    to = make(start, moves.moves[c]);
    //if illegal move
    if (to.boards[white] != 0x0ULL) {
      nodes += perft(to, depth - 1);
    }
  }
  return nodes;
}

void play(position position) {
  int move = 0;
  int color = position.turn;
  move_list moves;
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
      position = make(position, moves.moves[move]);
      //switch color
      color = get_opposite(color);
    }

  }
}



