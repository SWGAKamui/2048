#include <grid.h>
#include <strategy.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// inclure les strategies : 
#include "fast.h"
#include "fast2.h"
#include "hard.h"



void Remplir_Zero(grid g){
  // remplissage grille avec 0
  reset(g);
  for (int i = 0; i < GRID_SIDE ; i++){
    for (int j = 0; j < GRID_SIDE ; j++){
      set_tile(g,i,j,0);
    }
  }
  add_tile(g);
  add_tile(g);
}


int Max_tile(grid g){
  int max=0;
  for(int i = 0; i < GRID_SIDE; i++){
    for(int j= 0; j< GRID_SIDE; j++){
      if(max<get_tile(g,i,j)){
        max=get_tile(g,i,j);
      }
    }
  }
  return max;
}


int main (void){
  srand((unsigned int)time(NULL));
  //init grille
  grid g =new_grid();
  // remplissage grille avec 0 et deux tiles aléatoire
  
  Remplir_Zero(g);
  strategy strat1= fast(g);
  // Tant que la partie n'est pas fini, attendre input
  while(!game_over(g)) {
    dir direction=strat1->play_move(strat1,g);
    if (can_move(g,direction)){
      play(g,direction);              
      }
  }

  int score = grid_score(g);
  printf("score fast [Score : %5d]\n", score);
  int max = Max_tile(g);
  printf("tuile max fast [tuile : %5d]\n", max);
  
  // MAJ
  Remplir_Zero(g);
  // Autre stratégie
  strategy strat2= fast2(g);
  while(!game_over(g)) {
    dir direction=strat2->play_move(strat2,g);
    play(g,direction);
  }
  score = grid_score(g);
  printf("score fast2 [Score : %5d]\n", score);
  max = Max_tile(g);
  printf("tuile max fast2 [tuile : %5d]\n", max);
  
    // MAJ
  Remplir_Zero(g);
  // Autre stratégie
  strategy strat3= hard(g);
  while(!game_over(g)){
       
    dir direction=strat3->play_move(strat3,g);
    
    if (can_move(g,direction)){
      play(g,direction);              
    }
    else{
      int tmp=0;
      dir* list= list_ev(strat3,g);
      while(!can_move(g,direction)){
	direction = list[tmp];
	tmp ++;
      }
      play(g,direction);
    } 
  }

  score = grid_score(g);
  printf("score hard [Score : %5d]\n", score);
  max = Max_tile(g);
  printf("tuile max hard [tuile : %5d]\n", max);
  

  delete_grid(g);
  return EXIT_SUCCESS;  
}
