#include "precision.h"
#include "my_vector.h"
#include <stdio.h>

Vector3D::Vector3D(FLOAT x, FLOAT y, FLOAT z) { fx=x; fy=y; fz=z; }
void Vector3D::Set(FLOAT x, FLOAT y, FLOAT z) { fx=x; fy=y; fz=z; }
void Vector3D::SetX(FLOAT x) { fx=x; }
void Vector3D::SetY(FLOAT y) { fy=y; }
void Vector3D::SetZ(FLOAT z) { fz=z; }

FLOAT Vector3D::X(void) { return fx; }
FLOAT Vector3D::Y(void) { return fy; }
FLOAT Vector3D::Z(void) { return fz; }

/******************** OPERATORS ********************/

// Assignment
void Vector3D::operator=(Vector3D a) { fx=a.fx; fy=a.fy; fz=a.fz; }

// Scalar multiplication
Vector3D Vector3D::operator*(int c) {
  return Vector3D(c*fx, c*fy, c*fz);
}
Vector3D Vector3D::operator*(FLOAT c) {
  return Vector3D(c*fx, c*fy, c*fz);
}

// Scalar division
Vector3D Vector3D::operator/(int c) {
  float inv = 1.0/c;
  return Vector3D(fx*inv, fy*inv, fz*inv);
}
Vector3D Vector3D::operator/(FLOAT c) {
  FLOAT inv = 1.0/c;
  return Vector3D(fx*inv, fy*inv, fz*inv);
}

// Unary operators
Vector3D Vector3D::operator+(void) { return *this; }
Vector3D Vector3D::operator-(void) {
  return Vector3D(-fx, -fy, -fz);
}

// Regular operators
Vector3D Vector3D::operator+(int c) {
  return Vector3D(fx+c, fy+c, fz+c);
}
Vector3D Vector3D::operator-(int c) {
  return Vector3D(fx-c, fy-c, fz-c);
}
Vector3D Vector3D::operator+(FLOAT c) {
  return Vector3D(fx+c, fy+c, fz+c);
}
Vector3D Vector3D::operator-(FLOAT c) {
  return Vector3D(fx-c, fy-c, fz-c);
}
Vector3D Vector3D::operator+(Vector3D a) {
  return Vector3D(fx+a.fx, fy+a.fy, fz+a.fz);
}
Vector3D Vector3D::operator-(Vector3D a) {
  return Vector3D(fx-a.fx, fy-a.fy, fz-a.fz);
}

FLOAT Vector3D::operator*(Vector3D a) {
  return (fx*a.fx + fy*a.fy + fz*a.fz);
}
Vector3D Vector3D::operator^(Vector3D a) {
  return Vector3D(fy*a.fz - fz*a.fy, fz*a.fx-fx*a.fz, fx*a.fy - fy*a.fx);
}

// Modifier operators
Vector3D Vector3D::operator+=(Vector3D a) {
  Set(fx+a.fx, fy+a.fy, fz+a.fz);
  return *this;
}
Vector3D Vector3D::operator-=(Vector3D a) {
  Set(fx-a.fx, fy-a.fy, fz-a.fz);
  return *this;
}
Vector3D Vector3D::operator*=(FLOAT c) {
  Set(c*fx, c*fy, c*fz);
  return *this;
}
Vector3D Vector3D::operator/=(FLOAT c) {
  FLOAT inv = 1.0/c;
  Set(fx*inv, fy*inv, fz*inv);
  return *this;
}

/**************************************************/

FLOAT Vector3D::mag(void) {
  return sqrt(fx*fx + fy*fy + fz*fz);
}

void Vector3D::normalize(void) {
  float len = mag();
  fx /= len;
  fy /= len;
  fz /= len;
}

Vector3D Vector3D::Normal(void) {
  Vector3D a(fx,fy,fz);
  a.normalize();
  return a;
}

Vector3D Vector3D::Reset(void) {
  Set(0.0,0.0,0.0);
  return *this;
}

void Vector3D::info(void) {
  printf("%"FSYM" %"FSYM" %"FSYM"\n", fx, fy, fz);
}
