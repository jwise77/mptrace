#include <math.h>
#include "typedef.h"

void SetCameraPosition(param_t &p, FLOAT x[])
{
  for (int dim = 0; dim < MAX_DIMENSION; dim++)
    x[dim] = min(max(x[dim], 1e-3), 0.999);
  p.Camera.Set(x[0], x[1], x[2]);
  p.CameraDir = p.FocusPoint - p.Camera;
  p.DistanceToFocusPoint = p.CameraDir.mag();
  p.FarClipping = 2 * p.DistanceToFocusPoint;
  p.CameraDir.normalize();
  p.Camera.info();
  return;
}

void SetFocusPosition(param_t &p, FLOAT x[])
{
  for (int dim = 0; dim < MAX_DIMENSION; dim++)
    x[dim] = min(max(x[dim], 1e-3), 0.999);
  p.FocusPoint.Set(x[0], x[1], x[2]);
  p.CameraDir = p.FocusPoint - p.Camera;
  p.DistanceToFocusPoint = p.CameraDir.mag();
  p.FarClipping = 2 * p.DistanceToFocusPoint;
  p.CameraDir.normalize();
  return;
}

void RotateAroundFocus(param_t &p, FLOAT dtheta, int dir)
{

  int i;
  FLOAT e[3][3], newr[3];
  Vector3D dr;
  FLOAT alpha[3] = {0, 0, 0};

  dr = -p.CameraDir * p.DistanceToFocusPoint;

  /* Calculate Euler angles */
  alpha[dir] += dtheta;

  e[0][0] = +cos(alpha[2]) * cos(alpha[0]) - 
    cos(alpha[1]) * sin(alpha[0]) * sin(alpha[2]);
  e[1][0] = -sin(alpha[2]) * cos(alpha[0]) -
    cos(alpha[1]) * sin(alpha[0]) * cos(alpha[2]);
  e[2][0] = sin(alpha[1]) * sin(alpha[0]);

  e[0][1] = +cos(alpha[2]) * sin(alpha[0]) - 
    cos(alpha[1]) * cos(alpha[0]) * sin(alpha[2]);
  e[1][1] = -sin(alpha[2]) * sin(alpha[0]) -
    cos(alpha[1]) * cos(alpha[0]) * cos(alpha[2]);
  e[2][1] = -sin(alpha[1]) * cos(alpha[0]);

  e[0][2] = sin(alpha[2]) * sin(alpha[1]);
  e[1][2] = cos(alpha[2]) * sin(alpha[1]);
  e[2][2] = cos(alpha[1]);

  for (i = 0; i < MAX_DIMENSION; i++) {
    newr[i] = p.FocusPoint[i] +
      e[i][0] * dr[0] + e[i][1] * dr[1] + e[i][2] * dr[2];
  }

  SetCameraPosition(p, newr);

  return;
}
