#include <grid.h>
#include <strategy.h>
#include <stdlib.h>
#include <stdio.h>

dir alea(strategy s, grid g){
  dir t[4] = {UP,LEFT,RIGHT,DOWN};
  int alea = rand()%4;
  if (alea == 0 && can_move(g,t[alea])){
    s->mem=&t[0];
    return UP;
  }
  if (alea == 1 && can_move(g,t[alea])){
    s->mem=&t[1];
    return LEFT;
  }
  if (alea == 2 && can_move(g,t[alea])){
    s->mem=&t[2];
    return RIGHT;
  }
  if (alea == 3 && can_move(g,t[alea])){
    s->mem=&t[3];
    return DOWN;
  }
  int i=0;
  while (!can_move(g,t[i])){
    i++;
  }
  return t[i];
}

strategy fast(grid g){
  strategy fast =calloc (1,sizeof(*fast));
  fast->mem = NULL;
  fast->name = "fast";

  fast->play_move = alea;
  fast->free_strategy = free_memless_strat;
  return fast;
}

