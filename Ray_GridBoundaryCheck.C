#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"
#include "Ray.h"

void Ray::GridBoundaryCheck(bool &finished)
{

  int dim, iroot = INT_UNDEFINED;

  switch (this->CurrentGrid->GravityBoundaryType) {
  case TopGridPeriodic:
  case TopGridIsolated:
    for (dim = 0; dim < MAX_DIMENSION; dim++) {
      if (pos[dim] < this->CurrentGrid->leftEdge[dim])
	iroot = 2*dim;
      else if (pos[dim] > this->CurrentGrid->rightEdge[dim])
	iroot = 2*dim+1;
    }
    if (iroot != INT_UNDEFINED) {
      this->CurrentGrid = this->CurrentGrid->NeighborGrids[iroot];
      finished = (this->CurrentGrid == NULL);
    }
    break;
  case SubGridIsolated:
    this->CurrentGrid = this->CurrentGrid->ParentGrid;
    break;
  case GravityUndefined:
  default:
    fprintf(stdout, "Ray::GridBoundaryCheck - "
	    "GravityBoundaryType = %d undefined.\n", 
	    this->CurrentGrid->GravityBoundaryType);
    return;
  } // ENDSWITCH

  /* Check if ray is outside of domain */

  bool outside = false;
  for (dim = 0; dim < MAX_DIMENSION; dim++) {
    outside |= (pos[dim] < 0 || pos[dim] > 1);
  }
  finished = outside;

  return;

}
