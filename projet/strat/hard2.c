#include <grid.h>
#include <strategy.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


double evaluation2(grid g){
  double L=0;
  double US=0;

  double SUP_i=0;
  double SUP_j=0;
  double INF_i=0;
  double INF_j=0;

  
  for (int i=1; i<GRID_SIDE; i++){
    for (int j=1; j<GRID_SIDE; j++){
      L+=fabs(get_tile(g,i,j)-get_tile(g,i-1, j)) + fabs(get_tile(g,i,j)-get_tile(g,i, j-1));
    }
  }
  
  for (int i=0; i<GRID_SIDE; i++){
    for (int j=0; j<GRID_SIDE; j++){
      if (get_tile(g,i,j)>0){
	US++;
      }
    }
  }


  for (int i=0; i<GRID_SIDE; i++){
    for (int j=0; j<GRID_SIDE; j++){
      if(i-1>=0){
	if (get_tile(g,i,j)<get_tile(g,i-1,j)){
	  SUP_i++;
	}
	if (get_tile(g,i,j)>get_tile(g,i-1,j)){
	  INF_i++;
	}
      }
      if(j-1>=0){
	if (get_tile(g,i,j)<get_tile(g,i,j-1)){
	  SUP_j++;
	}
	if (get_tile(g,i,j)>get_tile(g,i,j-1)){
	  INF_j++;
	}
      }
    }
    }

  double M = min2(SUP_i, INF_i) + min2(SUP_j, INF_j);

  double e = 2*L + 2*US + 6*M;


  return e;
  
}


/* 
 __ __ __ __
|UP|DO|LE|RI| <- directions de départ
|? |? |? |? | <- si les direction peuvent s'effectuer, on regarde la meilleure ev qui suit
|? |? |? |? | <- si les direction peuvent s'effectuer, on regarde la meilleure ev qui suit
|? |? |? |? | <- si les direction peuvent s'effectuer, on regarde la meilleure ev qui suit
fin.


 */

void tri_bulles2(double tab[], dir tab2[])
{
    bool tab_en_ordre = false;
    int taille = 4;
    while(!tab_en_ordre){
      tab_en_ordre = true;
      for(int i=0 ; i < taille-1 ; i++){
	if(tab[i] > tab[i+1]){
	  int tmp;
	  dir dtmp;
	  tmp=tab[i];
	  tab[i]=tab[i+1];
	  tab[i+1]=tmp;

	  dtmp=tab2[i];
	  tab2[i]=tab2[i+1];
	  tab2[i+1]=dtmp;
	  
	  tab_en_ordre = false;
	}
      }
      taille--;
    }
}

enum dir best_dir(strategy s, grid g){
  // can move
  dir * t = malloc(4*sizeof(dir));
  t[0] = UP;
  t[1] = DOWN;
  t[2] = LEFT;
  t[3] = RIGHT;

  double * t2 = malloc(4*sizeof(double));
  grid g2 = new_grid();
  
  for (int i = 0; i<4; i++){
    g2 = copy_grid(g,g2);
    if (can_move(g2,t[i])){
	play(g2, t[i]);
	t2 = evaluation2(g2);
    }
  }
  tri_bulles(t2,t);
  
 
}


enum dir best_ev(strategy s, grid g){
  dir * final_tab = malloc(4*sizeof(dir));
  dir * tab_dir = malloc(4*sizeof(dir));
  for (int i = 0; i<4; i++){
    tab_dir[i] = malloc(4*sizeof(dir));
  }
  tab[0][0] = UP ;
  tab[0][1] = UP ;
  tab[0][2] = UP ;
  tab[0][3] = UP ;

  grid g2 = new_grid();
  

  double * tab_ev = malloc(4*sizeof(double));
  
  for (int j = 0; j<4; j++){
    g2 = copy_grid(g,g2);
    for (int i = 1; i<4; i++){
      if (can_move(g,tab_dir[j][i-1])){
	tab_ev[j]+=evaluation2(g2);
	play(g2, tab_dir[j,i]);
	
	tab_dir[j][i] = best_dir(s,g2);
	
      }
    }
  }

  
  
  // si la mémoire n'est pas vide, alors c'est qu'on a laissé un tableau de meilleurs directions
  dir final_d;
  if (s->mem != NULL){
    int i = 0;
    while (final_tab[i]==NULL){
      i++;
    }
    final_d = final_tab[i];
    final_tab[i]= NULL;
    if (i==3){
      free(s->mem);
      s->mem = NULL;
    }
  }


  return final_d;
}

















strategy hard2(grid g){ 
  strategy hard2 =calloc (1,sizeof(*hard2));
  hard2->mem = NULL;
  hard2->name = "hard2";

  hard2->play_move = best_ev2;
  hard2->free_strategy = free_memless_strat;
  return hard2;
}
