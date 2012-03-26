#include <stdlib.h>
#include <stdio.h>
#include "typedef.h"

int FieldsToLoad(param_t &p)
{

  if (p.RenderMode < _Density || p.RenderMode >= _Undefined) {
    printf("RenderMode = %d.  Must be between %d and %d.\n",
	   p.RenderMode, _Density, _Undefined);
    return EXIT_FAILURE;
  }

  switch (p.RenderMode) {
  case _Density:
  case _DensitySquared:
    p.NumberOfFields = 1;
    p.Fields[0] = Density;
    break;
  case _Isosurface:
    p.NumberOfFields = 2;
    p.Fields[0] = Density;
    p.Fields[1] = LogDensity;
    break;
  case _Photorealistic:
    p.NumberOfFields = 3;
    p.Fields[0] = Density;
    p.Fields[1] = Temperature;
    p.Fields[2] = LogDensity;
    break;
  } // ENDSWITCH

  return EXIT_SUCCESS;

}
