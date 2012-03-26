#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <queue>
#include "Grid.h"

#ifdef USE_HDF5
#include "hdf5.h"

void _sleep(const unsigned int millisec);
int FindField(int field, int farray[], int numfields);

int grid::LoadData(param_t p)
{

  char name[MAX_LINE_LENGTH], data_name[MAX_LINE_LENGTH];
  int i, dim, n, FieldNum, data_idx, size, num, pid;
  hid_t file_id, dset_id, start[] = {0,0,0};
  hid_t ndims = 3;
  hsize_t data_size[3];
  herr_t h5_error = -1;
  hid_t h5_status;
  hid_t file_dsp_id, group_id;

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
  if ((file_id = H5Fopen(this->filename, H5F_ACC_RDONLY, H5P_DEFAULT)) == HDF_FAIL) {
    fprintf(stderr, "Error opening file %s\n", this->filename);
    return EXIT_FAILURE;
  }

  char skip[] = "LogDensity";
  for (n = 0; n < p.NumberOfFields; n++) {

    // Skip log density because this is calculated when density is read
    int result = strcmp(p.FieldNames[p.Fields[n]], skip);
    if (result == 0) continue;

    /* Allocate memory and read data */

    if (this->cdata[n] == NULL)
      this->cdata[n] = new float[size];

    file_dsp_id = H5Screate_simple(ndims, data_size, NULL);
    if ( file_dsp_id == h5_error ) {
      fprintf(stderr, "Error creating namespace");
      return EXIT_FAILURE;
    }

    snprintf(name, MAX_LINE_LENGTH-1, "/Grid%8.8d", this->GroupID);
    strcpy(data_name, p.FieldNames[p.Fields[n]]);
//    printf("Grid %d: file = %s, group = %s, dataname = %s\n", 
//	   this->GroupID, this->filename, name, data_name);

    group_id = H5Gopen(file_id, name, H5P_DEFAULT);
    if( group_id == h5_error ) {
      fprintf(stderr, "Error opening group %s", name);
      return EXIT_FAILURE;
    }

    dset_id = H5Dopen(group_id, data_name, H5P_DEFAULT);
    if( dset_id == h5_error ) {
      fprintf(stderr, "Error opening dataset %s", data_name);
      return EXIT_FAILURE;
    }

    h5_status = H5Dread(dset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL,
			H5P_DEFAULT, (void*) this->cdata[n]);
    if (h5_status == h5_error) {
      fprintf(stderr, "Error reading dataset %s", data_name);
      return EXIT_FAILURE;
    }

    h5_status = H5Sclose(file_dsp_id);
    if (h5_status == h5_error) {
      fprintf(stderr, "Error closing dataset %s", data_name);
      return EXIT_FAILURE;
    }

    h5_status = H5Dclose(dset_id);
    if (h5_status == h5_error) {
      fprintf(stderr, "Error closing dataset %s", data_name);
      return EXIT_FAILURE;
    }

    h5_status = H5Gclose(group_id);
    if (h5_status == h5_error) {
      fprintf(stderr, "Error closing group %s", name);
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

  } // ENDFOR fields

  h5_status = H5Fclose(file_id);
  if (h5_status == h5_error) {
    fprintf(stderr, "Error closing file %s", this->filename);
    return EXIT_FAILURE;
  }
  
  /* Set subgrid marker field for this grid.  First itself and then
     its children */

  grid *Subgrid = this->NextGridNextLevel;
  this->SetSubgridMarker(this, this);
  while (Subgrid != NULL) {
    this->SetSubgridMarker(Subgrid, this);
    Subgrid = Subgrid->NextGridThisLevel;
  } // ENDWHILE subgrids

  /* Calculate vertex-centered values if using interpolation */

  if (p.UseInterpolation)
    this->ComputeVertexData(p.NumberOfFields);

  // Put this grid into the load queue and mark as loaded
  //p.LoadQueue.push((long long) this);
  num = (p.UseInterpolation) ? 2 : 1;
  p.memory_used += (double) ((num*p.NumberOfFields+1) * size * sizeof(float));
  this->loaded = true;
  this->in_use = false;

  return EXIT_SUCCESS;

}

#endif /* USE_HDF5 */
