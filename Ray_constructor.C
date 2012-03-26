#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Ray.h"

Ray::Ray(void)
{
  this->dir.Reset();
  this->ID = 0;
  this->radius = 0.0;
  this->intensity[0] = 0.0;
  this->intensity[1] = 0.0;
  this->intensity[2] = 0.0;
  this->CurrentGrid = NULL;
}
