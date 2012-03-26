#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#ifdef _OPENMP
#include <omp.h>
#endif
#include "Grid.h"
#include "Ray.h"
#include "colors.h"

int UpdateScreen(SDL_Surface *screen, param_t p, Ray *AllRays)
{

  int nrays, i, j, n;

  /* Calculate min/max of each band */

  float minI[] = {+1e20,+1e20,+1e20};
  float minI_nolog[] = {+1e20,+1e20,+1e20};
  float maxI[] = {-1e20,-1e20,-1e20};
  
  nrays = p.ImageSize[0] * p.ImageSize[1];
  for (i = 0; i < nrays; i++) {
    for (j = 0; j < 3; j++) {
      minI_nolog[j] = MIN(minI_nolog[j], AllRays[i].ReturnIntensity(j));
      if (AllRays[i].ReturnIntensity(j) > 0)
	minI[j] = MIN(minI[j], AllRays[i].ReturnIntensity(j));
      maxI[j] = MAX(maxI[j], AllRays[i].ReturnIntensity(j));
    }
  }

  if (p.RenderMode == _Density || p.RenderMode == _DensitySquared) {
    minI[0] = log10f(minI[0]);
    maxI[0] = log10f(maxI[0]);
  } else {
    minI[0] = minI_nolog[0];
  }

  /* Create RGB image from intensities */

  SDL_Surface *image;
  SDL_PixelFormat *fmt = screen->format;
  Uint8 thispix[3], color_bin;
  Uint32 *pixels = new Uint32[nrays];
  float RangeInv[3], RangeAInv;

  for (j = 0; j < 3; j++)
    if (maxI[j] > minI[j])
      RangeInv[j] = 1.0f / (maxI[j] - minI[j]);
    else
      RangeInv[j] = 1.0f;

  switch (p.RenderMode) {
  case _Isosurface:
  case _Photorealistic:
    for (n = 0; n < nrays; n++) {
      for (j = 0; j < 3; j++)
	thispix[j] = (Uint8) 
	  (255.0f * (AllRays[n].ReturnIntensity(j) - minI[j]) * RangeInv[j]);
      pixels[n] = SDL_MapRGBA(fmt, thispix[0], thispix[1], thispix[2], 255);
    }
    break;
  case _Density:
  case _DensitySquared:
    for (n = 0; n < nrays; n++) {
      color_bin = (Uint8) 
	(255.0f * (log10f(AllRays[n].ReturnIntensity(0)) - minI[0])
	 * RangeInv[0]);
      pixels[n] = SDL_MapRGBA(fmt, rgb[0][color_bin], rgb[1][color_bin], 
			      rgb[2][color_bin], 255);
    } // ENDFOR rays
    break;
  } // ENDSWITCH

  image = SDL_CreateRGBSurfaceFrom(pixels, p.ImageSize[0], p.ImageSize[1],
				   32, screen->pitch, fmt->Rmask, fmt->Gmask,
				   fmt->Bmask, fmt->Amask);
  if (image == NULL) {
    fprintf(stderr, "Could not create image: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (SDL_BlitSurface(image, NULL, screen, NULL) < 0) {
    SDL_FreeSurface(image);
    fprintf(stderr, "Could not blit image: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  //  SDL_SetAlpha(image, SDL_SRCALPHA, 255);
  SDL_UpdateRect(screen, NULL, NULL, image->w, image->h);
  SDL_FreeSurface(image);
  SDL_Flip(screen);

  delete [] pixels;

  return EXIT_SUCCESS;

}
