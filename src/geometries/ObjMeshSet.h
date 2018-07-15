#pragma once

#include <acceleration_structures/RegularGrid.h>
#include "GeometrySet.h"

/*
*   A geometry set representing an obj file, which would be split into multiple mesh geometries with different materials
*/

class ObjMeshSet : public RegularGrid
  {
public:
  const string filePath;
  ObjMeshSet(const string& filePath) : filePath(filePath) {}
  };
