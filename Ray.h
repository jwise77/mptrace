/*-*-C++-*-*/
#ifndef __RAY_H
#define __RAY_H
#include "Grid.h"

class Ray {

private:
  Vector3D dir;
  Vector3D pos;
  FLOAT radius;
  float intensity[3];
  float alpha;
  int ID;
  grid *CurrentGrid;

public:
  Ray();
  void Initialize(Vector3D u, Vector3D p, int ID, grid *g);
  int Trace(param_t p, bool &finished, 
	    void (*RenderFn) (FLOAT dr, float v[], float dI[]));
  void GridBoundaryCheck(bool &finished);
  void InterpolateValue(int gg[], int nFields, float dr, float value[]);

  grid* ReturnCurrentGrid(void) { return CurrentGrid; };
  void PrintInfo(void) {
    printf("r=%g, I[0]=%g, u=(%f %f %f), grid=%p\n", radius, intensity[0],
	   dir[0], dir[1], dir[2], CurrentGrid);
  };
  float ReturnIntensity(int n) { return intensity[n]; };
  float ReturnAlpha(void) { return alpha; };
  int ReturnID(void) { return ID; };

  inline void Nudge(Vector3D origin) {
    int dim;
    if (this->CurrentGrid != NULL)
      radius += 0.01*this->CurrentGrid->cellWidth;
    else
      radius += 1e-6;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      pos.SetIndex(dim, origin[dim] + radius * dir[dim]);
    return;
  }

  void NormalizeColor(void) {
    int i;
    for (i = 0; i < 3; i++) 
      if (alpha > 0) intensity[i] /= alpha;
    return;
  }

};

#endif
