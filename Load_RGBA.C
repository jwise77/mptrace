#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "typedef.h"
#include "colors.h"

int LoadCT(int table, unsigned char r[], unsigned char g[], unsigned char b[]);

int Load_RGBA(int table, render_type rtype)
{

  int i;
  float total;
  unsigned char rr[256], gg[256], bb[256];

  if (LoadCT(table, rr, gg, bb) == EXIT_FAILURE) {
    fprintf(stderr, "Error in LoadCT.\n");
    return EXIT_FAILURE;
  }

  // Normalize and store in RGB
  for (i = 0; i < 256; i++) {
    total = (float) (rr[i] + gg[i] + bb[i]);
    total = MAX(total, 1e-20);
    RGB[0][i] = rr[i] / total;
    RGB[1][i] = gg[i] / total;
    RGB[2][i] = bb[i] / total;
    rgb[0][i] = rr[i];
    rgb[1][i] = gg[i];
    rgb[2][i] = bb[i];
  }

  // Calculate SURFACE (gaussian stencil) if we are doing isosurfaces
  if (rtype == _Isosurface) {
    float factor, exponent, nalpha_2, sigma;
    nalpha_2 = (float) _NALPHA / 2;
    sigma = IsoSurfaceSigma * _NALPHA;
    factor = 0.39894228 / sigma;   // 1.0 / sqrt(2*PI) / sigma
    for (i = 0; i < _NALPHA; i++) {
      exponent = (i - nalpha_2) / sigma;
      SURFACE[i] = factor * expf(-0.5*exponent*exponent);
    }
  } else {
    for (i = 0; i < _NALPHA; i++)
      SURFACE[i] = 1.0;
  }

  return EXIT_SUCCESS;

}
