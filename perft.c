#include "perft.h"



U64 perft_enpassant(int move) {
  //get move flags to determine move type
  int promotion = decode_promotion(move);
  int capture = decode_capture(move);
  int y = decode_y(move);
  int z = decode_z(move);
  if ((!promotion)&&(capture)&&(!y)&&(z)) {
    return 1ULL;
  }
  return 0ULL;
}

void perft_test(char *in, int count, int depth) {

  init_attack();

  size_t buffer_size = 256;
  size_t line_size;
  size_t ptr_size;

  FILE *file = fopen(in, "r");
  char *line = (char *) malloc(buffer_size * sizeof(char));
  
  for (int c = 0; c < count; c++) {
    line_size = getline(&line, &buffer_size, file);
    //printf("%s", line);

    char delim[] = ",";

    //first call for strtok
    //get fen
    char *ptr = strtok(line, delim);
    ptr_size = strlen(ptr) + 1;
    char *fen = malloc(ptr_size);
    strcpy(fen, ptr);
    //printf("\n%s", fen);
    
    U64 expected[depth];
    for (int d = 0; d < depth; d++) {
      if (ptr != NULL) {
        ptr = strtok(NULL, delim);
        expected[d] = (U64) atoi(ptr);
      }
      else return;
    }

    //set position
    position pos;
    for (int c = 0; c < 8; c++) pos.boards[c] = 0x0ULL;
    for (int sq = 0; sq < 64; sq++) pos.square[sq] = 0;
    pos.turn = white;
    pos.castle = encode_castle(1, 1, 1, 1);
    pos.enpassant = encode_enpassant(0, 0);
    pos.index = 0;
    pos.undo->move = 0;
    pos.undo->castle = encode_castle(1, 1, 1, 1);
    pos.undo->enpassant = encode_enpassant(0, 0);
    pos.undo->captured = 0;
    parse(&pos, fen);
    bb_to_square(&pos);
    position position = pos;
/*     //print position
    printpos(pos);
    printgb(pos);

    for (int d = 0; d < depth; d++) {
      printf("\n\n@depth %d", d+1);
      printf("\nexpected : %d", expected[d]);
      printf("\nactual   : %lld", perft(&pos, game, d+1));
    } */
    U64 p;
    for (int d = 0; d < depth; d++) {
      pos = position;
      if ((p = perft(&pos, d+1)) != expected[d]) {
        
        printf("\n\n\n\n@line %d @depth %d", c+1, d+1);
        printf("\n%s", fen);

        printpos(position);
        printgb(position);
        
        printf("\n\nexpected : %lld", expected[d]);
        printf("\nactual   : %lld", p);
      }
    }
    

  }

}

U64 perft(position *pos, int depth) {
  
  U64 nodes = 0x0ULL;
  move_list moves;

  if (depth == 0) return 0x1ULL;
  
  moves = generate_moves(*pos);
  int size = moves.count;
  
  for (int c = 0; c < size; c++) {
    //if legal move
    if (!make(pos, moves.moves[c])) {
      nodes += perft(pos, depth - 1);
    }
    unmake(pos);
  }
  return nodes;
}

void play(position *pos) {
  
  int move = 0;
  int count = 0;
  move_list moves;
  while (move != -1) {

    //print board and moves
    printf("\n\n");
    printpos(*pos);
    printgb(*pos);
    moves = generate_moves(*pos);
    count = 0;
    for (int m = 0; m < moves.count; m++) {
      if (!make(pos, moves.moves[m])) {
        printf("\n%.3d:  ", m);
        printmove(moves.moves[m]);
        printf("   perft1: %lld   perft2: %lld", perft(pos, 1), perft(pos, 2));
        count++;
      }
      else {
        printf("\nXXX:  ");
        printmove(moves.moves[m]);
      }
      unmake(pos);
    }
    printf("\n\nmoves at position: %d\n\n", count);

    //get move from command line
    //get move
    printf("\n\nEnter Move Number: ");
    scanf("%d", &move);
    if (move == -2) {
      printf("\n\nUNMAKING MOVE\n\n");
      unmake(pos);
    }
    //make move
    else if (make(pos, moves.moves[move])) {
      printf("\n\nILLEGAL MOVE\n\n");
      unmake(pos);
    }
  }
}