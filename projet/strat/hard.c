#include <grid.h>
#include <strategy.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>


dir *initDirs()
{
  dir * tab_dir = malloc(4*sizeof(dir));
  tab_dir[0] = UP;
  tab_dir[1] = DOWN;
  tab_dir[2] = LEFT;
  tab_dir[3] = RIGHT;

  return tab_dir;
}

int min(int a, int b){
  if (a>b)
    return b;
  return a;
}

void tri_bulles(int tab[], dir tab2[])
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

int evaluation(grid g){
  int L=0;
  int US=0;

  int SUP_i=0;
  int SUP_j=0;
  int INF_i=0;
  int INF_j=0;

  
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

  int M = min(SUP_i, INF_i) + min(SUP_j, INF_j);

  int e = 2*L + 2*US + 6*M;


  return e;
  
}

double ev(strategy s, grid g, dir d){
  grid g2=new_grid();
  copy_grid(g,g2);
  s->mem = g2;
  play(s->mem, d);
  
  double e = evaluation(s->mem);

  free(g2);
  s->mem = NULL;

  return e;
}


dir * list_ev(strategy s, grid g){
  dir * tab_dir = initDirs();
  int* tab_ev = malloc(4*sizeof(int));;
  for (int i=0; i<4; i++){
    tab_ev[i]=ev(s,g,tab_dir[i]);
  }
  tri_bulles(tab_ev, tab_dir);
  free(tab_ev);
  return tab_dir;
}



dir best_ev(strategy s, grid g){
  dir *tab = initDirs();
  int e = ev(s,g, tab[0]);
  dir d=UP;
  int tmp;
  for (int i=1; i<4; i++){
    if (can_move(g,tab[i]))
      tmp = ev(s,g,tab[i]);
    if (e >= tmp){
      d = tab[i];
    }
    e = min(e,tmp);
  }
  if (!can_move(g,d)){
    int tmp=0;
    dir * list=initDirs();
    list = list_ev(s,g);
    while((!can_move(g,d)) || (tmp<4)){
      d = list[tmp];
      tmp ++;
    }
  }
  free(tab);
  return d;
}


strategy hard(grid g){ 
  strategy hard =calloc (1,sizeof(*hard));
  hard->mem = NULL;
  hard->name = "hard";

  hard->play_move = best_ev;
  hard->free_strategy = free_memless_strat;
  return hard;
}
