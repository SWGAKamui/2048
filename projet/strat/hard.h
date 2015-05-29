#include "../grid.h"
#include "strategy.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

enum dir best_ev(strategy s, grid g);
int ev(strategy s, grid g, dir d);
int evaluation(grid g);
int min(int a, int b);

dir *list_ev (strategy s, grid g);

strategy hard(grid g);
