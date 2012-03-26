#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

grid::grid()
{

  /* Null out fields */

  int i;
  for (i = 0; i < MAX_FIELDS; i++) {
    cdata[i] = NULL;
    vdata[i] = NULL;
  }
  for (i = 0; i < MAX_DIMENSION; i++) {
    leftEdge[i] = 0;
    rightEdge[i] = 0;
    dims[i] = 0;
  }
  FieldType = NULL;

  cellWidth	       = 0;
  dxinv		       = 0;
  level		       = -1;
  ID		       = 0;
  ParentID	       = 0;
  NumberOfBaryonFields = 0;
  npart		       = 0;
  loaded               = false;
  reloaded             = false;
  in_use               = false;

  NextGridThisLevel = NULL;
  NextGridNextLevel = NULL;
  ParentGrid	    = NULL;
  SubgridMarker     = NULL;
}
