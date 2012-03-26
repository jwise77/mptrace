#define DEBUG 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Grid.h"
#include "Ray.h"

int Ray::Trace(param_t p, bool &finished, 
	       void (*RenderFn) (FLOAT dr, float v[], float dI[]))
{

  int n, dim, u_dir[3], gg[3];
  FLOAT u_inv[3], ff[3], rr[3];
  FLOAT dx, dx_inv, diff;
  bool first_loop = true;
  int count = 0;

  // Ray position
  rr[0] = this->pos.X();
  rr[1] = this->pos.Y();
  rr[2] = this->pos.Z();
//  for (dim = 0; dim < MAX_DIMENSION; dim++)
//    rr[dim] = p.Camera[dim] + this->radius * this->dir[dim];

  // Check if ray is outside of the current grid
  if (this->CurrentGrid->PointOutside(rr)) {
    this->Nudge(p.Camera);
    this->GridBoundaryCheck(finished);
    if (DEBUG) {
      printf("[4] Ray %d: cells = %d, radius = %f, grid = %p\n", 
	     this->ID, count, this->radius, this->CurrentGrid);
      printf("\t rayp = %lf %lf %lf\n", pos.X(), pos.Y(), pos.Z());
      if (this->CurrentGrid != NULL) {
	printf("\t left = %lf %lf %lf\n", CurrentGrid->leftEdge[0],
	       CurrentGrid->leftEdge[1], CurrentGrid->leftEdge[2]);
	printf("\t rght = %lf %lf %lf\n", CurrentGrid->rightEdge[0],
	       CurrentGrid->rightEdge[1], CurrentGrid->rightEdge[2]);
      }
    }
    
    return EXIT_SUCCESS;
  } // ENDIF outside

  // For accessibility...
  dx = this->CurrentGrid->cellWidth;
  dx_inv = this->CurrentGrid->dxinv;

  // Ray direction, current cell in (int) and (FLOAT)
  for (dim = 0; dim < MAX_DIMENSION; dim++) {
    u_dir[dim] = (sign(this->dir[dim])+1)/2;
    gg[dim] = (int) ((rr[dim] - this->CurrentGrid->leftEdge[dim]) * dx_inv);
    ff[dim] = this->CurrentGrid->leftEdge[dim] + (FLOAT) gg[dim] * dx;

    // on cell boundaries the index will change in negative directions
    //    if (fabs(rr[dim]-ff[dim]) < ROUNDOFF)
    diff = fabs(rr[dim] - ff[dim]);
    //if (diff > 0.999) gg[dim] += (sign(this->dir[dim])+1)/2;
    if (diff < ROUNDOFF) gg[dim] += (sign(this->dir[dim])-1)/2;
  } // ENDIF dimension

  bool keep_tracing = true;
  int cindex, direction;
  FLOAT dr[3], min_dr, oldr, ddr, r;
  float vv[MAX_FIELDS], dI[4], alpha_factor;
  grid *oldgrid;

  while (keep_tracing) {
    count++;
    cindex = this->CurrentGrid->ReturnIndex(gg);

    // Check if inside a subgrid
    if (this->CurrentGrid->SubgridMarker[cindex] != this->CurrentGrid) {
      if (this->CurrentGrid->SubgridMarker[cindex] == NULL) {
	oldgrid = this->CurrentGrid;
	this->CurrentGrid = this->CurrentGrid->ParentGrid;
	//printf("%d", this->CurrentGrid->InMemory());
      } else {
	oldgrid = this->CurrentGrid;
	this->CurrentGrid = this->CurrentGrid->SubgridMarker[cindex];
	//printf("%d", this->CurrentGrid->InMemory());
      }
      this->Nudge(p.Camera);
      if (DEBUG) {
	printf("[3] Ray %d: cells = %d, grid = %p\n", this->ID, count, this->CurrentGrid);
	printf("\t rayp = %lf %lf %lf\n", pos.X(), pos.Y(), pos.Z());
	printf("\t left = %lf %lf %lf\n", CurrentGrid->leftEdge[0],
	       CurrentGrid->leftEdge[1], CurrentGrid->leftEdge[2]);
	printf("\t rght = %lf %lf %lf\n", CurrentGrid->rightEdge[0],
	       CurrentGrid->rightEdge[1], CurrentGrid->rightEdge[2]);
      }
      return EXIT_SUCCESS;
    } // ENDIF inside subgrid

    // Pre-compute 1.0/u_dir on first_loop
    if (first_loop) {
      for (dim = 0; dim < MAX_DIMENSION; dim++)
	u_inv[dim] = 1.0 / this->dir[dim];
      first_loop = false;
    }

    // Search for next cell-crossing
    min_dr = 1e20;
    oldr = this->radius;
    for (dim = 0; dim < MAX_DIMENSION; dim++) {
      dr[dim] = (this->CurrentGrid->leftEdge[dim] +
		 (gg[dim] + u_dir[dim]) * dx - 
		 p.Camera[dim]) * u_inv[dim];/// this->dir[dim];
      if (dr[dim] < min_dr) {
	min_dr = dr[dim];
	direction = dim;
      }
    } // ENDFOR dimension

    r = min_dr + ROUNDOFF;
    if (r >= p.FarClipping) {
      r = p.FarClipping;
      finished = true;
      keep_tracing = false;
      break;
    }
    ddr = r - oldr;

    if (r >= p.NearClipping) {

      if (p.UseInterpolation) {
	this->InterpolateValue(gg, p.NumberOfFields, ddr, vv);
      } else {
	for (n = 0; n < p.NumberOfFields; n++)
	  vv[n] = this->CurrentGrid->cdata[n][cindex];
	for (n = p.NumberOfFields; n < MAX_FIELDS; n++)
	  vv[n] = 0.0f;
      } // ENDELSE interpolation
      
      // Returns change in intensity (dI)
      RenderFn(ddr, vv, dI);
      if (p.RenderMode == _Isosurface || p.RenderMode == _Photorealistic) {
	//alpha_factor = dI[3] / (this->alpha+dI[3]);
	for (n = 0; n < 3; n++)
	  this->intensity[n] += dI[3] * dI[n];
	//this->intensity[n] += alpha_factor * dI[n];
	this->alpha += dI[3];
      } else {
	for (n = 0; n < 3; n++)
	  this->intensity[n] += dI[n];
      }

    } // ENDIF outside near clipping plane

    // Update ray position and radius
    this->radius += ddr;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      this->pos.SetIndex(dim, p.Camera[dim] + this->radius * this->dir[dim]);

    if (this->CurrentGrid->PointOutside(this->pos)) {
      rr[0] = this->pos.X();
      rr[1] = this->pos.Y();
      rr[2] = this->pos.Z();
      this->GridBoundaryCheck(finished);
      return EXIT_SUCCESS;
      if (DEBUG)
	printf("[2] Ray %d: cells = %d, grid = %p\n", this->ID, count, this->CurrentGrid);
    } // ENDIF outside
    
    // Update grid position
    gg[direction] += sign(this->dir[direction]);

  } // ENDWHILE keep_tracing

  if (DEBUG)
    printf("[1] Ray %d: cells = %d, grid = %p\n", this->ID, count, this->CurrentGrid);

  return EXIT_SUCCESS;

}
