#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Ray.h"

void Ray::Initialize(Vector3D u, Vector3D p, int ID, grid* g)
{
  this->dir = u;
  this->pos = p;
  this->ID = ID;
  this->radius = 0.0;
  this->intensity[0] = 0.0;
  this->intensity[1] = 0.0;
  this->intensity[2] = 0.0;
  this->alpha = 0.0;
  this->CurrentGrid = g;
  return;
}
