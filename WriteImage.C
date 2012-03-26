#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"
#include "Ray.h"
#include "hdf5.h"

int WriteImage(param_t p, Ray *AllRays)
{

  int i, n;
  int npixels = p.ImageSize[0] * p.ImageSize[1];
  float *image = new float[npixels];
  const char *dset_names[] = {"ImageR", "ImageG", "ImageB"};

  hsize_t OutDims[2] = {p.ImageSize[1], p.ImageSize[0]};
  hid_t file_id, dset_id, dspace_id, group_id;
  herr_t status;

  file_id = H5Fcreate("image.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  for (n = 0; n < 3; n++) {

    for (i = 0; i < npixels; i++)
      image[i] = AllRays[i].ReturnIntensity(n);

    dspace_id = H5Screate_simple(2, OutDims, NULL);
    dset_id = H5Dcreate(file_id, dset_names[n], H5T_NATIVE_FLOAT, 
			dspace_id, H5P_DEFAULT);
    if ((status = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL,
			   H5P_DEFAULT, image)) < 0) {
      fprintf(stderr, "Error writing image.\n");
      return EXIT_FAILURE;
    }
    H5Dclose(dset_id);
  } // ENDFOR n (RGB)

  H5Fclose(file_id);

  delete [] image;

  return EXIT_SUCCESS;

}
