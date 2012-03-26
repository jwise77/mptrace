/*-*-C++-*-*/
#ifndef __TYPEDEF_H
#define __TYPEDEF_H
#include <queue>
#include "precision.h"
#include "my_vector.h"
using namespace std;

//#define USE_HDF4
#define USE_HDF5

#define MAX_DIMENSION 3
#define MAX_LEVEL_OF_HIERARCHY 50
#define MAX_FIELDS 3
#define GHOST_ZONES 3
#define MAX_NUMBER_OF_BARYON_FIELDS 40

#define INT_UNDEFINED -99999
#define FLOAT_UNDEFINED -99999.0
#define MAX_LINE_LENGTH 256

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define HDF_FAIL -1

enum field_type {Density, TotalEnergy, InternalEnergy, Pressure,
		 Velocity1, Velocity2, Velocity3, 
		 ElectronDensity, HIDensity, HIIDensity,  HeIDensity, 
		 HeIIDensity, HeIIIDensity, HMDensity, H2IDensity, 
		 H2IIDensity, DIDensity, DIIDensity, HDIDensity,
                 Metallicity, kphHI, PhotoGamma, kphHeI,
		 kphHeII, kdissH2I, RadAccel1, RadAccel2, 
		 RadAccel3, SNColour, Temperature, LogDensity, FieldUndefined};

enum render_type {_Density, _DensitySquared, _Isosurface, _Photorealistic, 
		  _Undefined};

enum gravity_boundary_type {TopGridPeriodic, TopGridIsolated, SubGridIsolated, 
			    GravityUndefined};

enum movement_type {translate, rotate, focus};

#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define sign(A)  ((A) >  0  ?  1  : (-1))
#define nint(A) int((A) + 0.5*sign(A))
#define nlongint(A) ( (long_int) ((A) + 0.5*sign(A)) )

/* Parameters */

struct param_t {

  Vector3D Camera, FocusPoint, CameraDir, CameraUp;
  Vector3D cbasis1, cbasis2;
  FLOAT NearClipping, FarClipping, FieldOfView, DistanceToFocusPoint;
  double memory_used, max_memory;
  bool UseInterpolation;
  int MinLevel, MaxLevel;
  int ImageSize[2];
  int FindCenter;
  int nGrids;
  int RootResolution;
  int NumberOfParticles;
  render_type RenderMode;
  int ColorTable;
  int NumberOfFields;
  int Fields[MAX_FIELDS];
  char *FieldNames[MAX_NUMBER_OF_BARYON_FIELDS];
  int fps;
  movement_type mtype;
  FLOAT Time;
  //queue<long long> LoadQueue;

}; 

#endif /* __TYPEDEF_H */
