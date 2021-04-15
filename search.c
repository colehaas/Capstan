#include "search.h"

//alpha-beta search tree

//returns best move at a given depth
int search(position *pos, int depth) {



/*  
    * there are two types of nodes:
    *   - MAX nodes (your nodes)
    *   - MIN nodes (opponents nodes)
    * 
    * the goal is to get the best move at 
    * a desired depth
    * 
    * mininmize the MIN nodes and maximize the MAX nodes
    * 
    * a MIN parent finds the lowest MAX child
    * a MAX parent finds the highest MIN child
*/

    //depth counter
    //root node = 0, MAX node
    int d = 0;

/*
    * alpha -> minimum upper bound
    * beta -> maximum lower bound
    * thus, alpha <= (serached node) <= beta
    * 
    * at a MIN node the child (MAX node) with the lowest
    * value becomes that MIN's beta value
    * 
    * 
*/

    int alpha, beta;


    //search only efficient if moves are well ordered
    //they most certainly are not well ordered
    move_list moves = generate_moves(*pos);






}