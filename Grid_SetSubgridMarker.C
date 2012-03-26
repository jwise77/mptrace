#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

int grid::SetSubgridMarker(grid *Subgrid, grid *CurrentGrid)
{

  int i, j, k, index, dim;
  int SubgridStart[MAX_DIMENSION], SubgridEnd[MAX_DIMENSION];
  int size = this->ReturnSize();

  if (Subgrid != CurrentGrid)
    if (this->PointInside(Subgrid->leftEdge) == FALSE) {
      fprintf(stderr, "Subgrid left edge = (%"FSYM" %"FSYM" %"FSYM") "
	      "not inside parent grid?\n",
	      Subgrid->leftEdge[0], Subgrid->leftEdge[1], Subgrid->leftEdge[2]);
      fprintf(stderr, "Parent left edge = (%"FSYM" %"FSYM" %"FSYM")\n",
	      this->leftEdge[0], this->leftEdge[1], this->leftEdge[2]);
      return EXIT_FAILURE;
    }
  
  if (this->SubgridMarker == NULL) {
    this->SubgridMarker = new grid*[size];
    //if (CurrentGrid == Subgrid)
    for (i = 0; i < size; i++)
      this->SubgridMarker[i] = CurrentGrid;
    //
    //r (i = 0; i < size; i++)
    //this->SubgridMarker[i] = NULL;
  } // ENDIF SubgridMarker == NULL

  for (dim = 0; dim < MAX_DIMENSION; dim++) {
    SubgridStart[dim] = 
      nint((Subgrid->leftEdge[dim] - this->leftEdge[dim]) / this->cellWidth);
    SubgridEnd[dim] = 
      nint((Subgrid->rightEdge[dim] - this->leftEdge[dim]) / this->cellWidth)-1;
    SubgridStart[dim] = MAX(SubgridStart[dim], 0);
    SubgridEnd[dim] = MIN(SubgridEnd[dim], dims[dim]-1);
  }

  for (k = SubgridStart[2]; k <= SubgridEnd[2]; k++)
    for (j = SubgridStart[1]; j <= SubgridEnd[1]; j++) {
      index = this->ReturnIndex(SubgridStart[0], j, k);
      for (i = SubgridStart[0]; i <= SubgridEnd[0]; i++, index++)
	SubgridMarker[index] = Subgrid;
    } // ENDFOR j

  return EXIT_SUCCESS;

}
