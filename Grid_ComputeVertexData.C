#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

int grid::ComputeVertexData(int nFields)
{

  int i, j, k, n, dim, index, ci, vi[8];
  int im, ip, jm, jp, km, kp;
  int size = 1;
  for (dim = 0; dim < MAX_DIMENSION; dim++)
    size *= this->dims[dim] + 1;
  
  for (i = 0; i < nFields; i++) {
    if (this->vdata[i] == NULL)
      this->vdata[i] = new float[size];
    for (j = 0; j < size; j++)
      this->vdata[i][j] = 0.0;
  }
  for (i = nFields; i < MAX_FIELDS; i++)
    if (this->vdata[i] != NULL) {
      delete[] this->vdata[i];
      this->vdata[i] = NULL;
    }

  int dim0, dim1, dim2, side, indexp, indexo, psize;
  psize = 1;
  for (dim = 0; dim < MAX_DIMENSION; dim++)
    psize *= this->dims[dim] + 2;
  float *padded = new float[psize];

  for (n = 0; n < nFields; n++) {

    for (i = 0; i < psize; i++)
      padded[i] = 0.0;

    /* Create temporary padded field for easier interpolation */
    for (k = 0; k < dims[2]; k++) {
      for (j = 0; j < dims[1]; j++) {
	index = this->ReturnIndex(0, j, k);
	indexp = this->ReturnPadIndex(1, j+1, k+1);
	for (i = 0; i < dims[0]; i++, index++, indexp++) {
	  padded[indexp] = this->cdata[n][index];
	}
      }
    }

    /* Constant (slope=0) boundary conditions. */

    /* x-faces */

    for (k = 0; k < dims[2]+2; k++) {
      km = min(max(k-1, 0), dims[2]-1);
      for (j = 0; j < dims[1]+2; j++) {
	jm = min(max(j-1, 0), dims[1]-1);
	index = this->ReturnIndex(0, jm, km);
	indexp = this->ReturnPadIndex(0, j, k);
	padded[indexp] = this->cdata[n][index];
	index = this->ReturnIndex(dims[0]-1, jm, km);
	indexp = this->ReturnPadIndex(dims[0]+1, j, k);
	padded[indexp] = this->cdata[n][index];
      }
    }

    /* y-faces */

    for (k = 0; k < dims[2]+2; k++) {
      km = min(max(k-1, 0), dims[2]-1);
      for (i = 0; i < dims[0]+2; i++) {
	im = min(max(i-1, 0), dims[0]-1);
	index = this->ReturnIndex(im, 0, km);
	indexp = this->ReturnPadIndex(i, 0, k);
	padded[indexp] = this->cdata[n][index];
	index = this->ReturnIndex(im, dims[1]-1, km);
	indexp = this->ReturnPadIndex(i, dims[1]+1, k);
	padded[indexp] = this->cdata[n][index];
      }
    }

    /* z-faces */

    for (j = 0; j < dims[1]+2; j++) {
      jm = min(max(j-1, 0), dims[1]-1);
      for (i = 0; i < dims[0]+2; i++) {
	im = min(max(i-1, 0), dims[0]-1);
	index = this->ReturnIndex(im, jm, 0);
	indexp = this->ReturnPadIndex(i, j, 0);
	padded[indexp] = this->cdata[n][index];
	index = this->ReturnIndex(im, jm, dims[2]-1);
	indexp = this->ReturnPadIndex(i, j, dims[2]+1);
	padded[indexp] = this->cdata[n][index];
      }
    }

    /* Now we can interpolate from the padded data */

    ci = 0;
    // there is one more vertex per cell (1D)
    for (k = 0; k < dims[2]+1; k++) {
      for (j = 0; j < dims[1]+1; j++) {
	index = this->ReturnPadIndex(0, j, k);
	this->NeighborPadIndices(index, vi);
	for (i = 0; i < dims[0]+1; i++) {
	  this->vdata[n][ci++] = 0.125f *
	    (padded[vi[0]+i] + padded[vi[1]+i] +
	     padded[vi[2]+i] + padded[vi[3]+i] +
	     padded[vi[4]+i] + padded[vi[5]+i] +
	     padded[vi[6]+i] + padded[vi[7]+i]);
	} // ENDFOR i
      } // ENDFOR j
    } // ENDFOR k

  } // ENDFOR fields (n)

  delete[] padded;

  return EXIT_SUCCESS;

}

/**********************************************************************/

int grid::NeighborIndices(int index, int vi[])
{

  vi[0] = index;			// BOTTOM-SW (ORIGIN)
  vi[1] = index+1;			// BOTTOM-SE (i+1,j,k)
  vi[2] = index+dims[0];		// BOTTOM-NW (i,j+1,k)
  vi[3] = vi[2]+1;			// BOTTOM-NE (i+1,j+1,k)
  vi[4] = index+dims[0]*dims[1];	// TOP-SW (i,j,k+1)
  vi[5] = vi[4]+1;			// TOP-SE (i+1,j,k+1)
  vi[6] = vi[4]+dims[0];		// TOP-NW (i,j+1,k+1)
  vi[7] = vi[6]+1;			// TOP-NE (i+1,j+1,k+1)

  return EXIT_SUCCESS;

}

/**********************************************************************/

int grid::NeighborVertexIndices(int index, int vi[])
{

  int dim, vcdim[MAX_DIMENSION];
  for (dim = 0; dim < MAX_DIMENSION; dim++)
    vcdim[dim] = dims[dim] + 1;

  vi[0] = index;			// BOTTOM-SW (ORIGIN)
  vi[1] = index+1;			// BOTTOM-SE (i+1,j,k)
  vi[2] = index+vcdim[0];		// BOTTOM-NW (i,j+1,k)
  vi[3] = vi[2]+1;			// BOTTOM-NE (i+1,j+1,k)
  vi[4] = index+vcdim[0]*vcdim[1];	// TOP-SW (i,j,k+1)
  vi[5] = vi[4]+1;			// TOP-SE (i+1,j,k+1)
  vi[6] = vi[4]+vcdim[0];		// TOP-NW (i,j+1,k+1)
  vi[7] = vi[6]+1;			// TOP-NE (i+1,j+1,k+1)

  return EXIT_SUCCESS;

}

/**********************************************************************/

int grid::NeighborPadIndices(int index, int vi[])
{

  int dim, pdim[MAX_DIMENSION];
  for (dim = 0; dim < MAX_DIMENSION; dim++)
    pdim[dim] = dims[dim] + 2;

  vi[0] = index;			// BOTTOM-SW (ORIGIN)
  vi[1] = index+1;			// BOTTOM-SE (i+1,j,k)
  vi[2] = index+pdim[0];		// BOTTOM-NW (i,j+1,k)
  vi[3] = vi[2]+1;			// BOTTOM-NE (i+1,j+1,k)
  vi[4] = index+pdim[0]*pdim[1];	// TOP-SW (i,j,k+1)
  vi[5] = vi[4]+1;			// TOP-SE (i+1,j,k+1)
  vi[6] = vi[4]+pdim[0];		// TOP-NW (i,j+1,k+1)
  vi[7] = vi[6]+1;			// TOP-NE (i+1,j+1,k+1)

  return EXIT_SUCCESS;

}
