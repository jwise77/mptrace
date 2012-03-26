#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <queue>
#include "Grid.h"

#ifdef USE_HDF4
#include "mfhdf.h"

#define NO_COORDS_IN_FILE

void _sleep(const unsigned int millisec);
int FindField(int field, int farray[], int numfields);

int grid::LoadData(param_t p)
{

  int i, dim, n, FieldNum, data_idx, size, num, pid;
  int32 sd_id, sds_id, sds_index, start[] = {0,0,0};
  int32 data_size[3];

  // If another thread is reading or deleting, wait a little then continue.
//  while (this->in_use)
//    return EXIT_SUCCESS;

  // Now we're using it!
  this->in_use = true;

  // First delete any pre-existing data
  //this->DeleteData();

  // Number of cells
  size = this->ReturnSize();

  // HDF uses FORTRAN ordering
  for (dim = 0; dim < MAX_DIMENSION; dim++)
    data_size[3-dim-1] = this->dims[dim];

  // Open the file
  if ((sd_id = SDstart(this->filename, DFACC_RDONLY)) == HDF_FAIL) {
    fprintf(stderr, "Error opening file %s\n", this->filename);
    return EXIT_FAILURE;
  }

  for (n = 0; n < p.NumberOfFields; n++) {

    switch (p.Fields[n]) {
    case Temperature:
      FieldNum = NumberOfBaryonFields;
      break;
    case LogDensity:
      continue;
    default:
      FieldNum = FindField(p.Fields[n], this->FieldType, NumberOfBaryonFields);
      break;
    }

#ifdef COORDS_IN_FILE
    data_idx = 3*FieldNum+2;
#else
    data_idx = FieldNum;
#endif

    sds_id = SDselect(sd_id, data_idx);

    /* Allocate memory and read data */

    if (this->cdata[n] == NULL)
      this->cdata[n] = new float[size];
    if (SDreaddata(sds_id, start, (int32 *) NULL, data_size, 
		   (VOIDP) this->cdata[n]) == HDF_FAIL) {
      fprintf(stderr, "Error reading data from %s (field %d, sds %d)\n",
	      this->filename, p.Fields[n], sds_id);
      return EXIT_FAILURE;
    }

    // If in isosurface mode, we want the log values

    int nnn;
    if ((p.RenderMode == _Photorealistic || p.RenderMode == _Isosurface)
	&& p.Fields[n] == Density) {
      if (p.RenderMode == _Photorealistic)
	nnn = 2;
      if (p.RenderMode == _Isosurface)
	nnn = 1;
      if (this->cdata[nnn] == NULL)
	this->cdata[nnn] = new float[size];
      for (i = 0; i < size; i++)
	this->cdata[nnn][i] = log10f(this->cdata[n][i]);
    }

    SDendaccess(sds_id);

  } // ENDFOR fields
  
  SDend(sd_id);

  /* Set subgrid marker field for this grid.  First itself and then
     its children */

  grid *Subgrid = this->NextGridNextLevel;
  this->SetSubgridMarker(this, this);
  while (Subgrid != NULL) {
    this->SetSubgridMarker(Subgrid, this);
    Subgrid = Subgrid->NextGridThisLevel;
  } // ENDWHILE subgrids

  // Put this grid into the load queue and mark as loaded
  //p.LoadQueue.push((long long) this);
  num = (p.UseInterpolation) ? 2 : 1;
  p.memory_used += (double) ((num*p.NumberOfFields+1) * size * sizeof(float));
  this->loaded = true;
  this->in_use = false;

  return EXIT_SUCCESS;

}

#endif /* USE_HDF4 */
