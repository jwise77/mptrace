/*-*-C++-*-*/
#ifndef __GRID_H
#define __GRID_H

#include <stdlib.h>
#include "typedef.h"

struct LevelHierarchyEntry;

class grid
{

 private:

  /* Grid and particle data */

  float *cdata[MAX_FIELDS]; // cell-centered data
  float *vdata[MAX_FIELDS]; // vertex-centered data
  grid  **SubgridMarker;    // pointers to subgrids or itself for each cell

  /* Grid parameters */

  FLOAT	leftEdge[MAX_DIMENSION];
  FLOAT	rightEdge[MAX_DIMENSION];
  gravity_boundary_type GravityBoundaryType;
  FLOAT	cellWidth;
  FLOAT dxinv;
  int	level;
  int   ID;
  int   GroupID;
  int   ParentID;
  int	dims[MAX_DIMENSION];
  char  filename[80];
  bool  loaded;
  bool  reloaded;
  bool  in_use;
  
  int	 NumberOfBaryonFields;
  int	*FieldType;
  int	 npart;

  friend class Ray;

 public:

  /* Hierarchy pointers */
  
  grid  *NextGridThisLevel;
  grid  *NextGridNextLevel;
  grid  *ParentGrid;
  grid  *NeighborGrids[6];

  // Constructor
  grid();

  // Destructor
  ~grid();

  /* Routines written in separate files */

  int LoadData(param_t p);
  int ReadGridProperties(FILE *fptr, int CurrentLevel);
  int SetSubgridMarker(grid *Subgrid, grid *CurrentGrid);
  int FindNeighbors(LevelHierarchyEntry *LevelArray0);
  int ComputeVertexData(int nFields);
  int NeighborIndices(int index, int vi[]);
  int NeighborVertexIndices(int index, int vi[]);
  int NeighborPadIndices(int index, int vi[]);

  /* Routines assigned here */

  int ReturnLevel() { return level; };
  void AssignID(int value) { ID = value; };
  void AssignGID(int value) { GroupID = value; };
  int ReturnID() { return ID; };
  int ReturnGID() { return GroupID; };
  int ReturnNumberOfParticles() { return npart; };
  int ReturnNumberOfBaryonFields() { return NumberOfBaryonFields; };
  int InMemory() { return loaded; }
  int Reloaded() { return reloaded; }
  int InUse() { return in_use; }
  void SetUsage(bool value) { in_use = value; }
  void SetReload(bool value) { reloaded = value; }

  inline int ReturnIndex(int idx[]) {
    return (idx[2]*dims[1] + idx[1])*dims[0] + idx[0];
  };

  inline int ReturnIndex(int i, int j, int k) {
    return (k*dims[1] + j)*dims[0] + i;
  };

  inline int ReturnVCIndex(int i, int j, int k) {
    return (k*(dims[1]+1) + j)*(dims[0]+1) + i;
  };

  inline int ReturnPadIndex(int i, int j, int k) {
    return (k*(dims[1]+2) + j)*(dims[0]+2) + i;
  };

  inline int ReturnSize() {
    int dim, size = 1;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      size *= dims[dim];
    return size;
  };

  inline void ReturnFieldType(int values[]) {
    int i;
    for (i = 0; i < NumberOfBaryonFields; i++)
      values[i] = FieldType[i];
    return;
  };

  void ReturnInfo(int d[], FLOAT ledge[], FLOAT redge[], float &dx) {
    int dim;
    for (dim = 0; dim < MAX_DIMENSION; dim++) {
      d[dim] = dims[dim];
      ledge[dim] = leftEdge[dim];
      redge[dim] = rightEdge[dim];
    }
    dx = cellWidth;
    return;
  }; // END ReturnInfo

  // Useful for grids where the boundary is considered outside
  inline int PointOutside(FLOAT point[]) {
    int dim, outside = FALSE;
    for (dim = 0; dim < MAX_DIMENSION && !outside; dim++)
      outside |= point[dim] <= leftEdge[dim] || point[dim] >= rightEdge[dim];
    return outside;
  };

  // Useful for grids where the boundary is considered outside
  inline int PointOutsideDims(int point[]) {
    int dim, outside = FALSE;
    for (dim = 0; dim < MAX_DIMENSION && !outside; dim++)
      outside |= point[dim] < 0 || point[dim] >= dims[dim];
    return outside;
  };

  inline int PointOutside(Vector3D point) {
    int dim, outside = FALSE;
    for (dim = 0; dim < MAX_DIMENSION && !outside; dim++)
      outside |= (point[dim] <= leftEdge[dim] || point[dim] >= rightEdge[dim]);
    return outside;
  };

  // Use this one for regular checks, i.e. not rays
  inline int PointInside(FLOAT point[]) {
    int dim, inside = TRUE;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      inside &= point[dim] >= leftEdge[dim] && point[dim] <= rightEdge[dim];
    return inside;
  };

  inline int PointInside(Vector3D point) {
    int dim, inside = TRUE;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      inside &= point[dim] >= leftEdge[dim] && point[dim] <= rightEdge[dim];
    return inside;
  };

  void AdjustProperties(const int dx0) {
    int dim;
    this->dxinv = dx0 * (1 << this->level);
    this->cellWidth = 1.0 / this->dxinv;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      this->rightEdge[dim] = this->leftEdge[dim] + 
	this->dims[dim] * this->cellWidth;
    return;
  }

  void AssignFilename(char new_name[]) {
    char id[15];
    sprintf(id, ".grid%4.4d", ID);
    strcpy(filename, new_name);
    strcat(filename, id);
    return;
  };

  int IsNullGrid() {
    int dim;
    for (dim = 0; dim < MAX_DIMENSION; dim++)
      if (dims[dim] <= 1)
	return TRUE;
    return FALSE;
  };

  void DeleteData() {
    int i;
    for (i = 0; i < MAX_FIELDS; i++) {
      if (cdata[i] != NULL)
	delete [] cdata[i];
      if (vdata[i] != NULL)
	delete [] vdata[i];
      cdata[i] = NULL;
      vdata[i] = NULL;
    }
    if (SubgridMarker != NULL)
      delete [] SubgridMarker;
    SubgridMarker = NULL;
    loaded = false;
    return;
  }

};

struct LevelHierarchyEntry
{
  LevelHierarchyEntry *NextGridThisLevel;
  grid *Grid;
};

#endif /* __GRID_H */
