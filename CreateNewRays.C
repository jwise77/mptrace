#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"
#include "Ray.h"

int CreateNewRays(param_t &p, LevelHierarchyEntry *LevelArray[], Ray * &AllRays)
{

  int npixels = p.ImageSize[0] * p.ImageSize[1];

  if (AllRays == NULL)
    AllRays = new Ray[npixels];

  int i, j, n, index, dim;
  float resolution, fov, this_dir;
  Vector3D origin, cbasis1, cbasis2;

  // Field of view at the far clipping plane and resolution elements
  fov = 2.0 * p.FarClipping * tan(p.FieldOfView/2.0);
  resolution = fov / p.ImageSize[0];

  // Camera coordinate system basis (x,y = cbasis1, cbasis2)
  cbasis1 = p.CameraUp ^ p.CameraDir;
  cbasis1.normalize();
  cbasis2 = p.CameraDir ^ cbasis1;

  p.cbasis1 = cbasis1;
  p.cbasis2 = cbasis2;

  // Origin of far clipping plane
  for (dim = 0; dim < MAX_DIMENSION; dim++)
    origin.SetIndex(dim, p.Camera[dim] + p.CameraDir[dim] * p.FarClipping -
		    0.5*fov * cbasis1[dim] - 0.5*fov * cbasis2[dim]);

  /* Determine in which grid the camera is located */

  LevelHierarchyEntry *Temp;
  grid *CurrentGrid = NULL;
  int level, Found = FALSE;
  for (level = MAX_LEVEL_OF_HIERARCHY-1; level >= 0 && !Found; level--) {
    Temp = LevelArray[level];
    while (Temp != NULL && !Found)
      if ((Found = Temp->Grid->PointInside(p.Camera)) == FALSE)
	Temp = Temp->NextGridThisLevel;
  } // ENDFOR level

  CurrentGrid = Temp->Grid;

  /* Create one ray per pixel */

  Vector3D temp_vec1, temp_vec2;
  FLOAT r;

  index = 0;
  for (j = 0; j < p.ImageSize[1]; j++) {
    for (i = 0; i < p.ImageSize[0]; i++, index++) {

      for (dim = 0; dim < MAX_DIMENSION; dim++) {
	r = (origin[dim] + i*resolution*cbasis1[dim] + 
	     j*resolution*cbasis2[dim]) - p.Camera[dim];
	if (fabs(r) < ROUNDOFF) r = sign(r) * ROUNDOFF;
	temp_vec1.SetIndex(dim, r);
	temp_vec2.SetIndex(dim, p.Camera[dim]);
      }
      temp_vec1.normalize();
      AllRays[index].Initialize(temp_vec1, temp_vec2, index, CurrentGrid);

    } // ENDFOR i
  } // ENDFOR j

  return EXIT_SUCCESS;

}
