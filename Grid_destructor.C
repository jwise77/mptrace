#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

grid::~grid()
{

  int i;
  for (i = 0; i < MAX_FIELDS; i++) {
    if (cdata[i] != NULL)
      delete [] cdata[i];
    if (vdata[i] != NULL)
      delete [] vdata[i];
  }
  if (FieldType != NULL)
    delete [] FieldType;

}
