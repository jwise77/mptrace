#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Grid.h"
#include "Ray.h"

void Ray::InterpolateValue(int gg[], int nFields, float dr, 
			   float value[])
{
  
  int index0, n, dim, vi[8];
  float m[3], dr2;
  float *vd;

  dr2 = 0.5*dr;
  for (dim = 0; dim < MAX_DIMENSION; dim++) {
    m[dim] = (this->pos[dim] + dr2*this->dir[dim] - 
	      this->CurrentGrid->leftEdge[dim]) * this->CurrentGrid->dxinv;
    m[dim] -= int(m[dim]);
  }

  index0 = this->CurrentGrid->ReturnVCIndex(gg[0], gg[1], gg[2]);
  this->CurrentGrid->NeighborVertexIndices(index0, vi);

  for (n = 0; n < nFields; n++) {
    vd = this->CurrentGrid->vdata[n];
    value[n] = 
      vd[vi[0]] * (1-m[0])*(1-m[1])*(1-m[2]) +
      vd[vi[1]] * (  m[0])*(1-m[1])*(1-m[2]) +
      vd[vi[2]] * (1-m[0])*(  m[1])*(1-m[2]) +
      vd[vi[3]] * (  m[0])*(  m[1])*(1-m[2]) +
      vd[vi[4]] * (1-m[0])*(1-m[1])*(  m[2]) +
      vd[vi[5]] * (  m[0])*(1-m[1])*(  m[2]) +
      vd[vi[6]] * (1-m[0])*(  m[1])*(  m[2]) +
      vd[vi[7]] * (  m[0])*(  m[1])*(  m[2]);
  } // ENDFOR n (fields)

  for (n = nFields; n < MAX_FIELDS; n++)
    value[n] = 0.0;

  return;

}
