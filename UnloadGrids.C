#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <queue>
#include "Grid.h"

int UnloadGrids(param_t &p, LevelHierarchyEntry *LevelArray[])
{

  int num;
  grid *CurrentGrid = NULL;
  int ndelete = 0;

#ifdef UNUSED
  while (p.memory_used > p.max_memory) {

    CurrentGrid = (grid*) p.LoadQueue.front();
    CurrentGrid->SetUsage(true);
    p.LoadQueue.pop();

    CurrentGrid->DeleteData();
    CurrentGrid->SetUsage(false);
    num = (p.UseInterpolation) ? 2 : 1;
    p.memory_used -= (double) ((num*p.NumberOfFields+1) * sizeof(float) * 
			       CurrentGrid->ReturnSize());

    ndelete++;
    printf("nDelete = %d, memory_used = %g\n", ndelete, p.memory_used);

  } // ENDWHILE memory
#endif /* UNUSED */

  return EXIT_SUCCESS;

}
