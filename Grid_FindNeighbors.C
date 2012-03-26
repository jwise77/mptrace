#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

int grid::FindNeighbors(LevelHierarchyEntry *LevelArray0)
{

  int i, dim, side;
  FLOAT rr[MAX_DIMENSION];
  LevelHierarchyEntry *Temp;

  for (i = 0; i < 6; i++)
    this->NeighborGrids[i] = NULL;

  /* Minus and plus sides */

  for (side = 0; side < 2; side++) {

  for (dim = 0; dim < MAX_DIMENSION; dim++) {
    for (i = 0; i < MAX_DIMENSION; i++)
      if (i != dim)
	rr[i] = 0.5*(this->leftEdge[i] + this->rightEdge[i]);
      else if (side == 0)
	rr[i] = this->leftEdge[i] - this->cellWidth;
      else
	rr[i] = this->rightEdge[i] + this->cellWidth;

    if (rr[dim] < 0)
      rr[dim] += 1.0;
    else if (rr[dim] > 1)
      rr[dim] -= 1.0;
    for (Temp = LevelArray0; Temp; Temp = Temp->NextGridThisLevel) {
      if (Temp->Grid == this) continue;
      if (Temp->Grid->PointInside(rr) == TRUE) {
	this->NeighborGrids[2*dim+side] = Temp->Grid;
	break;
      }
    } // ENDFOR Temp
  }

  } // ENDFOR side

  return EXIT_SUCCESS;

}
