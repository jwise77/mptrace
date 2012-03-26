#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

int GetGrid(LevelHierarchyEntry *LevelArray[], int level, int num, 
	    grid * &return_grid)
{

  if (num < 0 || level < 0 || level > MAX_LEVEL_OF_HIERARCHY) {
    fprintf(stderr, "Invalid num (%d) or level (%d) in GetGrid.\n", num, level);
    return EXIT_FAILURE;
  }

  int i;
  LevelHierarchyEntry *Temp = LevelArray[level];
  for (i = 0; i <= num; i++) {
    if (Temp == NULL) {
      fprintf(stderr, "GetGrid :: Bad num = %d (level %d)\n", num, level);
      return EXIT_FAILURE;
    }
    Temp = Temp->NextGridThisLevel;
  }

  return_grid = Temp->Grid;
  return EXIT_SUCCESS;

}
