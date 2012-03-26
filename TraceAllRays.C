#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "Grid.h"
#include "Ray.h"
#include "render.h"

int UnloadGrids(param_t &p, LevelHierarchyEntry *LevelArray[]);

int TraceAllRays(param_t p, LevelHierarchyEntry *LevelArray[], Ray *&AllRays,
		 bool reload)
{

  int i, j, n;
  bool finished;
  grid *CurrentGrid;
  void (*RenderFn) (FLOAT dr, float v[], float dI[]);

  switch (p.RenderMode) {
  case _Density:
    RenderFn = Render_Density;
    break;
  case _DensitySquared:
    RenderFn = Render_DensitySquared;
    break;
  case _Isosurface:
    RenderFn = Render_Isosurface;
    break;
  case _Photorealistic:
    RenderFn = Render_Photorealistic;
    break;
  default:
    fprintf(stderr, "Unknown render mode = %d.\n", p.RenderMode);
    return EXIT_FAILURE;
  } // ENDSWITCH render mode

  int nrays = p.ImageSize[0] * p.ImageSize[1];
  int loop;
  int ErrorSignal = 0;
  int nload = 0;

  printf("Progress: [");
  for (i = 0; i < 61; i++)
    printf(" ");
  printf("]");
  for (i = 0; i < 62; i++)
    printf("\b");

#ifdef _OPENMP
#pragma omp parallel private(finished, loop, CurrentGrid)
#endif /* _OPENMP */
  for (i = 0; i < nrays; i++) {
    
    finished = false;
    loop = 0;

    while (!finished) {

      CurrentGrid = AllRays[i].ReturnCurrentGrid();

#ifdef _OPENMP
#pragma omp critical
{
#endif
      // If necessary, (un)load the data
      if (!CurrentGrid->InMemory() || 
	  (reload && !CurrentGrid->Reloaded())) {
	CurrentGrid->LoadData(p);
	if (reload) CurrentGrid->SetReload(true);
	nload++;
	//printf("%d grids loaded\n", nload);
      }
#ifdef _OPENMP
}
#endif

      if (p.memory_used > p.max_memory)
	if (UnloadGrids(p, LevelArray) == EXIT_FAILURE) {
	  fprintf(stderr, "Error in UnloadGrids.\n");
	}

      /* Trace ray through current grid.  Inside we check if the ray
	 enters another grid. */

      if (AllRays[i].Trace(p, finished, RenderFn) == EXIT_FAILURE) {
	fprintf(stderr, "Error in Ray::Trace.\n");
      }

      loop++;

    } // ENDWHILE !finished

    /* Update progress bar and viewer */

    if (i % (nrays/60) == 0) {
      printf("-");
      fflush(stdout);
    }

  } // ENDFOR rays
  
  printf("]\n");

  return EXIT_SUCCESS;

}
