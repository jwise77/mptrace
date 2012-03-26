#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

void AddLevel (LevelHierarchyEntry *LevelArray[], grid *Grid, int level)
{

  LevelHierarchyEntry *ThisLevel;
  ThisLevel = new LevelHierarchyEntry;
  ThisLevel->Grid = Grid;
  ThisLevel->NextGridThisLevel = LevelArray[level];
  LevelArray[level] = ThisLevel;

  /* recursively call this for the next grid on the same level */
  
  if (Grid->NextGridThisLevel != NULL)
    AddLevel(LevelArray, Grid->NextGridThisLevel, level);
  
  /* And now finer levels... */
  if (Grid->NextGridNextLevel != NULL)
    AddLevel(LevelArray, Grid->NextGridNextLevel, level+1);

}
