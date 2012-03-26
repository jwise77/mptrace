#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"
#include "colors.h"

/* -------------------- Function Prototypes -------------------- */
int ReadHierarchyEntry(FILE *fptr, grid * &Grid, int GridID, grid *ParentGrid,
		       int CurrentLevel);
void AddLevel (LevelHierarchyEntry *LevelArray[], grid *Grid, int level);
int Load_RGBA(int table, render_type rtype);
int FieldsToLoad(param_t &p);
int DefineFieldNames(param_t &p);
/* ------------------------------------------------------------- */
int ReadGridInfo (char *basename, param_t &p, grid * &Grids, 
		  LevelHierarchyEntry *LevelArray[])
{

  FILE *fptr;
  char hierName[80], filename[80];
  char line[256], dummyStr[256];
  int i, j, k, index, dim, size, level;
  int parentID;
  int temp_int;

  /* Read parameters from enzo parameter file */

  if ((fptr = fopen(basename, "r")) == NULL) {
    fprintf(stderr, "Error opening parameter file %s\n", basename);
    return EXIT_FAILURE;
  }

  while (fgets(line, 256, fptr) != NULL) {
    sscanf(line, "InitialTime = %"FSYM, &p.Time);
    sscanf(line, "TopGridDimensions = %d", &p.RootResolution);
    sscanf(line, "NumberOfParticles = %d", &p.NumberOfParticles);
  }

  fclose(fptr);  

  /* Initialize defaults for other parameters */

  float length = 0;
  p.Camera.Set(0.1, 0.5, 0.5);
  //p.Camera.Set(0.171788, 0.267338, 0.226687);
  //p.FocusPoint.Set(0.183, 0.231, 0.184);
  p.FocusPoint.Set(0.5, 0.5, 0.5);
  p.CameraUp.Set(0,0,1);
  p.CameraDir = p.FocusPoint - p.Camera;
  length = p.CameraDir.mag();
  p.DistanceToFocusPoint = length;
  p.CameraDir.normalize();

  p.NearClipping = 0.01 * length;
  p.FarClipping = 2*length;
  p.FieldOfView = 60.0 * (M_PI / 180);   // radians
  p.ImageSize[0] = 200;
  p.ImageSize[1] = 200;
  p.memory_used = 0.0;
  p.max_memory = 2000e6;                  // in bytes
  p.fps = 30;
  p.mtype = translate;
  //p.RenderMode = _Photorealistic;
  p.RenderMode = _DensitySquared;
  //p.RenderMode = _Isosurface;
  p.ColorTable = 4;
  p.MinLevel = INT_UNDEFINED;
  p.MaxLevel = INT_UNDEFINED;
  p.UseInterpolation = true;
  IsoSurfaceRange[0] = -0.5;
  IsoSurfaceRange[1] = 2.0;
  IsoSurfaceSigma = 0.1;
  IsoSurfaceDelta = 0.5;
  PhotoRealisticBrightness = 1.0;
  PhotoRealisticRange[0] = 1000;
  PhotoRealisticRange[1] = 30000;


  /* Load field names */

  if (DefineFieldNames(p) == EXIT_FAILURE) {
    fprintf(stderr, "Error in DefineFieldNames.\n");
    return EXIT_FAILURE;
  }

  if (FieldsToLoad(p) == EXIT_FAILURE) {
    fprintf(stderr, "Error in FieldsToLoad.\n");
    return EXIT_FAILURE;
  }

  IsoSurfaceRangeDiff = IsoSurfaceRange[1] - IsoSurfaceRange[0];
  PhotoRealisticRangeDiff = PhotoRealisticRange[1] - PhotoRealisticRange[0];
  PhotoRealisticRangeDiffInv = 1.0 / PhotoRealisticRangeDiff;
  if (Load_RGBA(p.ColorTable, p.RenderMode) == EXIT_FAILURE) {
    fprintf(stderr, "Error in Load_RGBA.\n");
    return EXIT_FAILURE;
  }

  /* Read .hierarchy file */

  strcpy(hierName, basename);
  strcat(hierName, ".hierarchy");
  
  if ((fptr = fopen(hierName, "r")) == NULL) {
    fprintf(stderr, "Error opening hierarchy %s\n", hierName);
    return EXIT_FAILURE;
  }

  grid *temp = NULL;
  int dummy, GridID = 1;

  fprintf(stdout, "Reading %s...\n", hierName);
  if (ReadHierarchyEntry(fptr, Grids, GridID, NULL, 0) == EXIT_FAILURE) {
    fprintf(stderr, "Error in ReadHierarchyEntry.\n");
    return EXIT_FAILURE;
  }
  fclose(fptr);

  /* Create LevelArray and re-assign grid IDs based on position inside
     the array. */

  AddLevel(LevelArray, Grids, 0);
  p.nGrids = 0;
  LevelHierarchyEntry *Temp;
  for (level = 0; level < MAX_LEVEL_OF_HIERARCHY; level++) {
    Temp = LevelArray[level];
    dummy = 0;
    if (Temp != NULL) {
      if (p.MinLevel == INT_UNDEFINED)
	p.MinLevel = level;
      p.MaxLevel = level;
    }
    while (Temp != NULL) {
      Temp->Grid->AssignID(dummy);
      Temp->Grid->AdjustProperties(p.RootResolution);
      p.nGrids++;
      dummy++;
      Temp = Temp->NextGridThisLevel;
    }
  }

  /* Find neighbor grids for root level */

  Temp = LevelArray[0];
  while (Temp != NULL) {
    Temp->Grid->FindNeighbors(LevelArray[0]);
    Temp = Temp->NextGridThisLevel;
  }

  fprintf(stdout, "--> Read %d grids and %d particles from %s.\n", 
	  p.nGrids, p.NumberOfParticles, hierName);
  fprintf(stdout, "--> Maximum level %d\n\n", p.MaxLevel);

  p.memory_used = p.nGrids * sizeof(grid);
  
  return EXIT_SUCCESS;

}
