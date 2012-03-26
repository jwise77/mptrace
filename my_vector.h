#ifndef __MY_VECTOR_H
#define __MY_VECTOR_H

#include <math.h>
#define Xi 0
#define Yi 1
#define Zi 2

class Vector3D
{
 private:
  FLOAT fx, fy, fz;
 public:
  Vector3D() {fx=0.0; fy=0.0; fz=0.0;}
  Vector3D(FLOAT x, FLOAT y, FLOAT z);
  void Set(FLOAT x, FLOAT y, FLOAT z);
  void SetX(FLOAT x);
  void SetY(FLOAT y);
  void SetZ(FLOAT z);
  inline void SetIndex(int index, FLOAT value) {
    switch (index) {
    case Xi: fx=value;
    case Yi: fy=value;
    case Zi: fz=value;
    }
  };
  FLOAT X(void);
  FLOAT Y(void);
  FLOAT Z(void);
  FLOAT mag(void);
  void normalize(void);
  Vector3D Normal(void);
  Vector3D Reset(void);
  void info(void);

  void operator=(Vector3D a);
  Vector3D operator+(void);
  Vector3D operator-(void);

  Vector3D operator+(int c);
  Vector3D operator-(int c);
  Vector3D operator+(FLOAT c);
  Vector3D operator-(FLOAT c);
  Vector3D operator*(int c);
  Vector3D operator*(FLOAT c);
  Vector3D operator/(int c);
  Vector3D operator/(FLOAT c);
  Vector3D operator+(Vector3D a);
  Vector3D operator-(Vector3D a);

  FLOAT operator*(Vector3D a);  // dot product
  Vector3D operator^(Vector3D a);  // cross product

  Vector3D operator+=(Vector3D a);
  Vector3D operator-=(Vector3D a);
  Vector3D operator/=(FLOAT c);
  Vector3D operator*=(FLOAT c);

  inline FLOAT operator[](short index) {
    switch(index) {
    case Xi: return fx;
    case Yi: return fy;
    case Zi: return fz;
    }
    return 0.0;  // if invalid index
  };
  
};

#endif
