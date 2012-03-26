#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"

int ReadHierarchyEntry(FILE *fptr, grid * &Grid, int GridID, grid *ParentGrid,
		       int CurrentLevel)
{

  int TestGridID, NextGridThisLevelID, NextGridNextLevelID;

  /* Read header info for this grid */

  if (fscanf(fptr, "\nGrid = %d\n", &TestGridID) != 1) {
    fprintf(stderr, "Error reading Grid # in grid %d.\n", GridID);
    return EXIT_FAILURE;
  }

  if (TestGridID != GridID) {
    fprintf(stderr, "Unexpected GridID = %d while reading grid %d.\n", 
	    TestGridID, GridID);
    return EXIT_FAILURE;
  }

  /* Create new grid and fill out hierarchy entry. */

  Grid->NextGridThisLevel = NULL;
  Grid->NextGridNextLevel = NULL;
  Grid->ParentGrid        = ParentGrid;

  Grid->AssignGID(GridID);
  Grid->AssignID(GridID);
  Grid->ReadGridProperties(fptr, CurrentLevel);

  if (fscanf(fptr, "Pointer: Grid[%d]->NextGridThisLevel = %d\n", &TestGridID,
	     &NextGridThisLevelID) != 2) {
    fprintf(stderr, "Error reading NextGridThisLevel pointer for grid %d.\n",
	    GridID);
    return EXIT_FAILURE;
  }
  if (TestGridID != GridID) {
    fprintf(stderr, "GridID = %d does not match grid(1) %d.\n", 
	    TestGridID, GridID);
    return EXIT_FAILURE;
  }

  /* If the pointer was non-zero, then read that grid. */

  if (NextGridThisLevelID != 0) {
    Grid->NextGridThisLevel = new grid;
    if (ReadHierarchyEntry(fptr, Grid->NextGridThisLevel, NextGridThisLevelID,
			   ParentGrid, CurrentLevel) == EXIT_FAILURE) {
      fprintf(stderr, "Error in ReadDataHierarchy(1).\n");
      return EXIT_FAILURE;
    }
  }

  /* Read pointer information for the next grid next level. */

  if (fscanf(fptr, "Pointer: Grid[%d]->NextGridNextLevel = %d\n",
	     &TestGridID, &NextGridNextLevelID) != 2) {
    fprintf(stderr, "Error reading NextGridNextLevel pointer for grid %d.\n",
	    GridID);
    return EXIT_FAILURE;
  }
  if (TestGridID != GridID) {
    fprintf(stderr, "GridID = %d does not match grid(2) %d.\n", 
	    TestGridID, GridID);
    return EXIT_FAILURE;
  }

  /* If the pointer was non-zero, then read that grid. */

  if (NextGridNextLevelID != 0) {
    Grid->NextGridNextLevel = new grid;
    if (ReadHierarchyEntry(fptr, Grid->NextGridNextLevel, NextGridNextLevelID,
			   Grid, CurrentLevel+1) == EXIT_FAILURE) {
      fprintf(stderr, "Error in ReadDataHierarchy(2).\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;

}
