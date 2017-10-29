#pragma once
//
// Created by matherno on 13/09/17.
//


#include <AccelerationStructure.h>

class RegularGrid : public AccelerationStructure {
private:
  float cellNumFactor = 2;

  std::vector<GeometryPtr> cells;
  std::vector<uint> cellCounts;
  uint numCellsX = 0;
  uint numCellsY = 0;
  uint numCellsZ = 0;

public:
  RegularGrid(float cellNumFactor = 2);
  virtual ~RegularGrid(){}

  virtual bool hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, float* hitTValue) const override;

protected:
  virtual void initialiseStructure() override;

  uint getCellIndex(int x, int y, int z) const;
  void addGeometryToCell(GeometryPtr geometry, int minCellX, int minCellY, int minCellZ, int maxCellX, int maxCellY, int maxCellZ);
  void getCellContainingPoint(const Vector3D& point, int* cellX, int* cellY, int* cellZ) const;
};
