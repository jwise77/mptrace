#ifdef DEFINE_STORAGE
#define EXTERN
#else
#define EXTERN extern
#endif

#ifndef __COLORS_H
#define __COLORS_H
#define _NALPHA 256
#endif

EXTERN float RGB[3][256];
EXTERN unsigned char rgb[3][256];
EXTERN float SURFACE[_NALPHA];
EXTERN float IsoSurfaceRange[2];
EXTERN float IsoSurfaceRangeDiff;
EXTERN float IsoSurfaceDelta;
EXTERN float IsoSurfaceSigma;
EXTERN float PhotoRealisticBrightness;
EXTERN float PhotoRealisticRange[2];
EXTERN float PhotoRealisticRangeDiff;
EXTERN float PhotoRealisticRangeDiffInv;
