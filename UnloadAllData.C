#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

void UnloadAllData(param_t &p, LevelHierarchyEntry *LevelArray[])
{

  int level;
  LevelHierarchyEntry *Temp;

  /* Reset load queue */

  //while (!p.LoadQueue.empty())
  //  p.LoadQueue.pop();

  /* Delete data */

  for (level = 0; level < MAX_LEVEL_OF_HIERARCHY; level++) {
    Temp = LevelArray[level];
    while (Temp != NULL) {
      if (Temp->Grid->InMemory())
	Temp->Grid->DeleteData();
      Temp = Temp->NextGridThisLevel;
    } // ENDWHILE grids
  } // ENDFOR level

  p.memory_used = p.nGrids * sizeof(grid);

  return;

}
