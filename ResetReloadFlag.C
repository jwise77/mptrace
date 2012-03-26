#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

void ResetReloadFlag(LevelHierarchyEntry *LevelArray[])
{

  int level;
  LevelHierarchyEntry *Temp;

  /* Delete data */

  for (level = 0; level < MAX_LEVEL_OF_HIERARCHY; level++)
    for (Temp = LevelArray[level]; Temp; Temp = Temp->NextGridThisLevel)
      Temp->Grid->SetReload(false);

  return;

}
