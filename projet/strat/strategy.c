#include "strategy.h"
#include "stdlib.h"

void free_memless_strat (strategy strat)
{
  free (strat);
}
