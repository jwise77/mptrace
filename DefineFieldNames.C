#include <stdlib.h>
#include <stdio.h>
#include "typedef.h"

int DefineFieldNames(param_t &p)
{

  p.FieldNames[Density] = (char*) "Density";
  p.FieldNames[TotalEnergy] = (char*) "TotalEnergy";
  p.FieldNames[InternalEnergy] = (char*) "GasEnergy";
  p.FieldNames[Pressure] = (char*) "Pressure";
  p.FieldNames[Velocity1] = (char*) "x-velocity";
  p.FieldNames[Velocity2] = (char*) "y-velocity";
  p.FieldNames[Velocity3] = (char*) "z-velocity";
  p.FieldNames[ElectronDensity] = (char*) "Electron_Density";
  p.FieldNames[HIDensity] = (char*) "HI_Density";
  p.FieldNames[HIIDensity] = (char*) "HII_Density";
  p.FieldNames[HeIDensity] = (char*) "HeI_Density";
  p.FieldNames[HeIIDensity] = (char*) "HeII_Density";
  p.FieldNames[HeIIIDensity] = (char*) "HeIII_Density";
  p.FieldNames[HMDensity] = (char*) "HM_Density";
  p.FieldNames[H2IDensity] = (char*) "H2I_Density";
  p.FieldNames[H2IIDensity] = (char*) "H2II_Density";
  p.FieldNames[DIDensity] = (char*) "DI_Density";
  p.FieldNames[HDIDensity] = (char*) "HDI_Density";
  p.FieldNames[Metallicity] = (char*) "Metal_Density";
  p.FieldNames[kphHI] = (char*) "HI_kph";
  p.FieldNames[PhotoGamma] = (char*) "PhotoGamma";
  p.FieldNames[kphHeI] = (char*) "HeI_kph";
  p.FieldNames[kphHeII] = (char*) "HeII_kph";
  p.FieldNames[kdissH2I] = (char*) "H2I_kdiss";
  p.FieldNames[RadAccel1] = (char*) "RadAccel1";
  p.FieldNames[RadAccel2] = (char*) "RadAccel2";
  p.FieldNames[RadAccel3] = (char*) "RadAccel3";
  p.FieldNames[SNColour] = (char*) "SN_Colour";
  p.FieldNames[Temperature] = (char*) "Temperature";
  p.FieldNames[LogDensity] = (char*) "LogDensity";
  p.FieldNames[FieldUndefined] = (char*) "";
//  p.FieldNames[] = "";

  return EXIT_SUCCESS;

}
