/******************************************************************************
  INTERACTIVE RAY TRACER FOR ENZO
  Created by John H. Wise (September, 2008)

  Features: 
  -- Given an image size, ray trace to each pixel from a far-clipping plane
  -- Provide many different color maps (i.e. from IDL)
  -- Modes: total column density and photo-realistic rendering
  -- Camera position interactions
  -- Camera path editor (with options to see rough renderings and/or 
     grid boundaries)
  
  Strategies:
  -- Dynamically load/unload data into memory for speed
  -- Load cell-based data, interpolate to vertex, then keep in memory
  -- Trace one ray per pixel, using vertex interpolation
  -- While zooming-in interactively, zoom in with OpenGL while creating the new
     ray-trace, like Google Earth loads its images.
  -- Only use data that has a resolution equal or larger than a pixel width.
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "Grid.h"
#include "Ray.h"
#define DEFINE_STORAGE
#include "colors.h"
#undef DEFINE_STORAGE

void print_help(void);
int ReadGridInfo(char *basename, param_t &p, grid * &Grids, 
		 LevelHierarchyEntry *LevelArray[]);
void UnloadAllData(param_t &p, LevelHierarchyEntry *LevelArray[]);
void ResetReloadFlag(LevelHierarchyEntry *LevelArray[]);
int CreateNewRays(param_t &p, LevelHierarchyEntry *LevelArray[], Ray *&AllRays);
int TraceAllRays(param_t p, LevelHierarchyEntry *LevelArray[], Ray *&AllRays,
		 bool reload);
int UpdateScreen(SDL_Surface *screen, param_t p, Ray *AllRays);
void InterpretKeyPress(SDL_keysym *keysym, param_t &p, bool &quit, 
		       bool &redraw, bool &reload);

int main(int argc, char *argv[])
{

  int level;
  char basename[80];

  param_t parameters;
  grid *Grids = new grid;
  Ray *AllRays = NULL;
  bool quit = false, isActive = true, redraw = true, reload = false;
  bool WriteFrames = false, DisplaySequence = false;
  SDL_Event event;

  LevelHierarchyEntry *LevelArray[MAX_LEVEL_OF_HIERARCHY];
  for (level = 0; level < MAX_LEVEL_OF_HIERARCHY; level++)
    LevelArray[level] = NULL;

  /* Interpret command line options */
  
  if (argc < 2) {
    fprintf(stderr, "Usage: %s amr_file\n", argv[0]);
    return EXIT_FAILURE;
  }
  strcpy(basename, argv[1]);

  /* Read the skeleton hierarchy (i.e. no data) */

  if (ReadGridInfo(basename, parameters, Grids, LevelArray) == EXIT_FAILURE) {
    fprintf(stderr, "Error in ReadGridInfo.\n");
    return EXIT_FAILURE;
  }

  /* Initialize SDL and viewing window */

  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Surface *screen;
  screen = SDL_SetVideoMode(parameters.ImageSize[0], parameters.ImageSize[1], 
			    32, SDL_SWSURFACE|SDL_ANYFORMAT);
  if (screen == NULL) {
    fprintf(stderr, "Could not set %dx%dx24 video mode: %s\n",
	    parameters.ImageSize[0], parameters.ImageSize[1], 
	    SDL_GetError());
    quit = true;
  }

  /* Now that we have loaded the dataset hierarchy, we can enter the
     main loop that addresses: 
     1. Casting new rays
     2. Tracing rays
     3. Dynamical load/unload/interpolation of data
     4. Image creation
     5. Previews of AMR box outlines and low-res images
  */
  
  print_help();

  while (!quit) {
    
    if (redraw) {

      if (reload) {
	ResetReloadFlag(LevelArray);
	UnloadAllData(parameters, LevelArray);
      }
//      if (unload) {
//	UnloadAllData(parameters, LevelArray);
//	unload = false;
//      }

      /* Cast new rays from the camera */
    
      if (CreateNewRays(parameters, LevelArray, AllRays) == EXIT_FAILURE) {
	fprintf(stderr, "Error in CreateNewRays.\n");
	break;
      }

      /* Trace rays through grids and load/unload them as necessary */

      if (TraceAllRays(parameters, LevelArray, AllRays, 
		       reload) == EXIT_FAILURE) {
	fprintf(stderr, "Error in TraceAllRays.\n");
	break;
      }

      /* Update the display */

      if (UpdateScreen(screen, parameters, AllRays) == EXIT_FAILURE) {
	fprintf(stderr, "Error in UpdateDisplay.\n");
	break;
      }

    } // ENDIF active

    redraw = false;
    reload = false;
    while (SDL_PollEvent( &event )) {
      switch( event.type ) {
      case SDL_ACTIVEEVENT:
	isActive = (event.active.gain == 0) ? false : true;
	break;

      case SDL_KEYDOWN:
	InterpretKeyPress( &event.key.keysym, parameters, 
			   quit, redraw, reload );
	break;
	
      case SDL_QUIT:
	quit = true;
	break;

      default:
	break;
      } // ENDSWITCH
    } // ENDWHILE

    if (DisplaySequence && !WriteFrames)
      SDL_Delay(1000.0f / (float) parameters.fps);
    else if (!WriteFrames)
      SDL_Delay(33.3333f); // 60 fps

  } // ENDWHILE !quit

  SDL_Quit();

  return EXIT_SUCCESS;

}
