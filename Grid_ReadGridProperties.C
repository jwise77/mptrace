#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

int ReadListOfInts(FILE *fptr, int N, int nums[]);

void throw_away_line(FILE *fptr, int n) {
  char dummy[256];
  int i;
  for (i = 0; i < n; i++)
    fgets(dummy, 256, fptr);
  return;
}

int grid::ReadGridProperties(FILE *fptr, int CurrentLevel)
{

  char dummyStr[256];
  int dim, dummy;
  //FLOAT dx;

  throw_away_line(fptr,2); // Task, GridRank

  if (fscanf(fptr, "GridDimension = %d %d %d\n", &dims[0], &dims[1], 
	     &dims[2]) == 3)
    for (dim = 0; dim < 3; dim++) dims[dim] -= 2*GHOST_ZONES;

  throw_away_line(fptr,2); // Start/End

  fscanf(fptr, "GridLeftEdge = %"FSYM" %"FSYM" %"FSYM"\n", &leftEdge[0], 
	 &leftEdge[1], &leftEdge[2]);

  fscanf(fptr, "GridRightEdge = %"FSYM" %"FSYM" %"FSYM"\n", 
	 &rightEdge[0], &rightEdge[1], &rightEdge[2]);

  throw_away_line(fptr,2); // Time / Subgrids

  if (fscanf(fptr, "NumberOfBaryonFields = %d\n", &NumberOfBaryonFields) == 1) {
    FieldType = new int[NumberOfBaryonFields];
    fscanf(fptr, "FieldType = ");
    if (ReadListOfInts(fptr, NumberOfBaryonFields, FieldType) == EXIT_FAILURE) {
      fprintf(stderr, "Error reading FieldType (grid %d)\n", ID);
      return EXIT_FAILURE;
    }
  }

  if (fscanf(fptr, "BaryonFileName = %s\n", dummyStr) == 1) {

    // Calculate derived quantities
    cellWidth = (FLOAT) (rightEdge[0] - leftEdge[0]) / dims[0]; 
    dxinv = 1.0 / cellWidth;
    level = CurrentLevel;
    strcpy(filename, dummyStr);

  } // ENDIF read Baryon file name

  throw_away_line(fptr,4); // cfl & params
  fscanf(fptr, "NumberOfParticles = %d\n", &npart);
  if (npart > 0)
    throw_away_line(fptr,1); // particle filename
  fscanf(fptr, "GravityBoundaryType = %d\n", &GravityBoundaryType);
  //throw_away_line(fptr,1); // gravity boundary

  return EXIT_SUCCESS;

}
