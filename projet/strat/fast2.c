#include <grid.h>
#include <strategy.h>
#include <stdlib.h>
#include <stdio.h>

dir cycle(strategy s, grid g){
  dir tab[4] = {UP,LEFT,DOWN,RIGHT};
  for(int i=0; i<4; i++){
    if(s->mem == NULL && can_move(g,tab[0])){
      s->mem = &tab[0];
      return tab[0];
    }
    if (s->mem == &tab[i] && can_move(g,tab[(i+1)%4])){
      s->mem = &tab[(i+1)%4];
      return tab[(i+1)%4];
    }
  }
  // si aucune valeur n'a été retournée (can_move false)
  int i=0;
  while (!can_move(g,tab[i])){
    i++;
  }
  return tab[i];

}

strategy fast2(grid g){
  strategy fast2 =calloc (1,sizeof(*fast2));
  fast2->mem = NULL;
  fast2->name = "fast2";

  fast2->play_move = cycle;
  fast2->free_strategy = free_memless_strat;
  return fast2;
}
