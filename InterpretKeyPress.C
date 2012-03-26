#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SDL.h"
#include "typedef.h"
#include "colors.h"

int FieldsToLoad(param_t &p);
int Load_RGBA(int table, render_type rtype);
void SetCameraPosition(param_t &p, FLOAT x[]);
void SetFocusPosition(param_t &p, FLOAT x[]);
void RotateAroundFocus(param_t &p, FLOAT dtheta, int dir);
void print_help(void);

#define MOVE 0.1

void InterpretKeyPress(SDL_keysym *keysym, param_t &p, bool &quit, 
		       bool &redraw, bool &reload)
{

  const FLOAT delta_r = (FLOAT) MOVE * p.DistanceToFocusPoint;
  const FLOAT delta_th = MOVE;  // radians

  int i, dim, input_int, old_value;
  char input[256], input1[256], input2[256];
  FLOAT newr[3];

  switch (keysym->sym) {
  case SDLK_q:
  case SDLK_ESCAPE:
    quit = true;
    break;

  case SDLK_h:
    print_help();
    break;

  case SDLK_m:
    printf("1. Translate\n"
	   "2. Rotate\n"
	   "3. Change focus point\n"
	   "Change movement type: ");
    scanf("%s", input);
    input_int = atoi(input);
    if (input_int < 1 || input_int > 3)
      printf("Invalid choice.  Movement type remaining at %d\n", p.mtype);
    else
      p.mtype = (movement_type) (input_int-1);
    break;

  case SDLK_f:
    printf("Set new focus point (x y z): ");
    scanf("%s %s %s", input, input1, input2);
    newr[0] = atof(input);
    newr[1] = atof(input1);
    newr[2] = atof(input2);
    SetFocusPosition(p, newr);
    redraw = true;
    break;

  case SDLK_UP:
    switch (p.mtype) {
    case translate:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.Camera[dim] - delta_r * p.cbasis2[dim];
      SetCameraPosition(p, newr);
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] - delta_r * p.cbasis2[dim];
      SetFocusPosition(p, newr);
      break;
    case rotate:
      RotateAroundFocus(p, delta_th, 1);
      break;
    case focus:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] - delta_r * p.cbasis2[dim];
      SetFocusPosition(p, newr);
      break;
    } // ENDSWITCH
    redraw = true;
    break;

  case SDLK_DOWN:
    switch (p.mtype) {
    case translate:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.Camera[dim] + delta_r * p.cbasis2[dim];
      SetCameraPosition(p, newr);
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] + delta_r * p.cbasis2[dim];
      SetFocusPosition(p, newr);
      break;
    case rotate:
      RotateAroundFocus(p, -delta_th, 1);
      break;
    case focus:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] + delta_r * p.cbasis2[dim];
      SetFocusPosition(p, newr);
      break;
    } // ENDSWITCH
    redraw = true;
    break;

  case SDLK_LEFT:
    switch (p.mtype) {
    case translate:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.Camera[dim] + delta_r * p.cbasis1[dim];
      SetCameraPosition(p, newr);
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] + delta_r * p.cbasis1[dim];
      SetFocusPosition(p, newr);
      break;
    case rotate:
      RotateAroundFocus(p, -delta_th, 0);
      break;
    case focus:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] + delta_r * p.cbasis1[dim];
      SetFocusPosition(p, newr);
      break;
    } // ENDSWITCH
    redraw = true;
    break;

  case SDLK_RIGHT:
    switch (p.mtype) {
    case translate:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.Camera[dim] - delta_r * p.cbasis1[dim];
      SetCameraPosition(p, newr);
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] - delta_r * p.cbasis1[dim];
      SetFocusPosition(p, newr);
      break;
    case rotate:
      RotateAroundFocus(p, delta_th, 0);
      break;
    case focus:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] - delta_r * p.cbasis1[dim];
      SetFocusPosition(p, newr);
      break;
    } // ENDSWITCH
    redraw = true;
    break;

  case SDLK_PAGEDOWN:
    switch (p.mtype) {
    case rotate:
      RotateAroundFocus(p, -delta_th, 2);
      break;
    case focus:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] + delta_r * p.CameraDir[dim];
      SetFocusPosition(p, newr);
      break;
    default:
      break;
    } // ENDSWITCH
    redraw = true;
    break;
    
  case SDLK_PAGEUP:
    switch (p.mtype) {
    case rotate:
      RotateAroundFocus(p, delta_th, 2);
      break;
    case focus:
      for (dim = 0; dim < 3; dim++)
	newr[dim] = p.FocusPoint[dim] - delta_r * p.CameraDir[dim];
      SetFocusPosition(p, newr);
      break;
    default:
      break;
    } // ENDSWITCH
    redraw = true;
    break;

  case SDLK_a:
    for (dim = 0; dim < 3; dim++)
      newr[dim] = p.Camera[dim] + delta_r * p.CameraDir[dim];
    SetCameraPosition(p, newr);
    redraw = true;
    break;

  case SDLK_z:
    for (dim = 0; dim < 3; dim++)
      newr[dim] = p.Camera[dim] - delta_r * p.CameraDir[dim];
    SetCameraPosition(p, newr);
    redraw = true;
    break;

  case SDLK_c:
    printf("Change color type [0-40,100, current=%d]: ", p.ColorTable);
    scanf("%s", input);
    input_int = atoi(input);
    if ((input_int < 0 || input_int > 40) && input_int != 100)
      printf("Invalid choice.  Color table remaining at %d\n", p.ColorTable);
    else
      p.ColorTable = input_int;
    Load_RGBA(p.ColorTable, p.RenderMode);
    redraw = true;
    break;

  case SDLK_r:
    old_value = (int) p.RenderMode;
    printf("1. Density Projection\n"
	   "2. Density^2 Projection\n"
	   "3. Isosurfaces\n"
	   "4. Photorealistic Rendering\n"
	   "Change movement type: ");
    scanf("%s", input);
    input_int = atoi(input);
    if (input_int < 1 || input_int > 4)
      printf("Invalid choice.  Render mode remaining at %d\n", p.mtype);
    else
      p.RenderMode = (render_type) (input_int-1);
    if (p.RenderMode == _Photorealistic)
      p.ColorTable = 100;
    if (old_value == (int) _Photorealistic && 
	p.RenderMode != _Photorealistic)
      p.ColorTable = 4;
    Load_RGBA(p.ColorTable, p.RenderMode);
    if (old_value != p.RenderMode) {
      reload = true;
    }
    if (FieldsToLoad(p) == EXIT_FAILURE) {
      fprintf(stderr, "Error in FieldsToLoad.\n");
      quit = true;
    }
    redraw = true;
    break;

  case SDLK_COMMA:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceRange[0] -= 0.1*IsoSurfaceRangeDiff;
      IsoSurfaceRangeDiff = IsoSurfaceRange[1] - IsoSurfaceRange[0];
      printf("Isosurface range = %f %f\n", IsoSurfaceRange[0], 
	     IsoSurfaceRange[1]);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_PERIOD:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceRange[0] += 0.1*IsoSurfaceRangeDiff;
      IsoSurfaceRangeDiff = IsoSurfaceRange[1] - IsoSurfaceRange[0];
      printf("Isosurface range = %f %f\n", IsoSurfaceRange[0], 
	     IsoSurfaceRange[1]);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_k:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceRange[1] -= 0.1*IsoSurfaceRangeDiff;
      IsoSurfaceRangeDiff = IsoSurfaceRange[1] - IsoSurfaceRange[0];
      printf("Isosurface range = %f %f\n", IsoSurfaceRange[0], 
	     IsoSurfaceRange[1]);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_l:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceRange[1] += 0.1*IsoSurfaceRangeDiff;
      IsoSurfaceRangeDiff = IsoSurfaceRange[1] - IsoSurfaceRange[0];
      printf("Isosurface range = %f %f\n", IsoSurfaceRange[0], 
	     IsoSurfaceRange[1]);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_MINUS:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceDelta -= 0.25*IsoSurfaceDelta;
      printf("Isosurface delta = %f\n", IsoSurfaceDelta);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_PLUS:
  case SDLK_EQUALS:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceDelta += 0.25*IsoSurfaceDelta;
      printf("Isosurface delta = %f\n", IsoSurfaceDelta);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_9:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceSigma -= 0.5*IsoSurfaceSigma;
      printf("Isosurface sigma = %f\n", IsoSurfaceSigma);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_0:
    if (p.RenderMode == _Isosurface) {
      IsoSurfaceSigma += 0.5*IsoSurfaceSigma;
      printf("Isosurface sigma = %f\n", IsoSurfaceSigma);
      Load_RGBA(p.ColorTable, p.RenderMode);
      redraw = true;
    }
    break;

  case SDLK_i:
    p.UseInterpolation = !p.UseInterpolation;
    reload = true;
    redraw = true;
    break;

  } // ENDSWITCH

  return;

}
