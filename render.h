#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "typedef.h"
#include "colors.h"

inline void Render_Density(FLOAT dr, float v[], float dI[]) {
  dI[0] = dr * v[0];
  dI[1] = 0.0;
  dI[2] = 0.0;
  dI[3] = 1e-6;
}

inline void Render_DensitySquared(FLOAT dr, float v[], float dI[]) {
  dI[0] = dr * v[0] * v[0];
  dI[1] = 0.0;
  dI[2] = 0.0;
  dI[3] = 1e-6;
}

inline void Render_Isosurface(FLOAT dr, float v[], float dI[]) {
  int i, alpha_bin, color_bin;
  float alpha_float, alpha_remainder;
  alpha_float = (v[1] - IsoSurfaceRange[0]) / IsoSurfaceDelta;
  if (alpha_float > 0) {
    //alpha2 = powf(alpha_float, 0.5f);
    alpha_remainder = alpha_float - (int) alpha_float;
    alpha_bin = (int) (alpha_remainder * _NALPHA);
    color_bin = (int) (255.9f * (v[1] - IsoSurfaceRange[0]) / IsoSurfaceRangeDiff);
    color_bin = MIN(color_bin, 255);
    for (i = 0; i < 3; i++)
      dI[i] = RGB[i][color_bin];
    dI[3] = dr * alpha_float * SURFACE[alpha_bin];
  } else {
    for (i = 0; i < 4; i++)
      dI[i] = 0.0;
  }
}

inline void Render_Photorealistic(FLOAT dr, float v[], float dI[]) {
  int i, color_bin;
  float alpha;
  alpha = v[2] - IsoSurfaceRange[0];
  if (alpha > 0 && v[1] > PhotoRealisticRange[0]) {
    color_bin = (int) (255.9f * (v[1] - PhotoRealisticRange[0]) *
		       PhotoRealisticRangeDiffInv);
    color_bin = MIN(color_bin, 255);
    for (i = 0; i < 3; i++)
      dI[i] = RGB[i][color_bin];
    dI[3] = dr * v[0];
  } else
    for (i = 0; i < 4; i++)
      dI[i] = 0.0;
}

