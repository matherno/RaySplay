//
// Created by matherno on 13/09/17.
//

#include <shaders/FlatColourShader.h>
#include <cfloat>
#include "RegularGrid.h"

RegularGrid::RegularGrid(float cellNumFactor /*= 2*/) : cellNumFactor(cellNumFactor) {

}

#define CLAMP(x, y) mathernogl::clampi((int)x, 0, y)

//  if outside of grid, this just clamps the cell numbers
void RegularGrid::getCellContainingPoint(const Vector3D& point, int* cellX, int* cellY, int* cellZ) const {
  float gridSizeX, gridSizeY, gridSizeZ;
  boundingBox->getSize(&gridSizeX, &gridSizeY, &gridSizeZ);
  Vector3D cell = point - boundingBox->getLowerBound();
  cell.x *= (float) numCellsX / gridSizeX;
  cell.y *= (float) numCellsY / gridSizeY;
  cell.z *= (float) numCellsZ / gridSizeZ;
  *cellX = CLAMP(cell.x, numCellsX-1);
  *cellY = CLAMP(cell.y, numCellsY-1);
  *cellZ = CLAMP(cell.z, numCellsZ-1);
}

void RegularGrid::initialiseStructure() {
  float gridSizeX, gridSizeY, gridSizeZ;
  boundingBox->getSize(&gridSizeX, &gridSizeY, &gridSizeZ);
  float s = (float)pow(gridSizeX*gridSizeY*gridSizeZ / size(), 0.3333);
  numCellsX = (uint) std::max((cellNumFactor * gridSizeX / s) + 1, 1.0f);
  numCellsY = (uint) std::max((cellNumFactor * gridSizeY / s) + 1, 1.0f);
  numCellsZ = (uint) std::max((cellNumFactor * gridSizeZ / s) + 1, 1.0f);
  uint numCells = numCellsX * numCellsY * numCellsZ;

  cells.clear();
  cells.reserve(numCells);
  cellCounts.clear();
  cellCounts.reserve(numCells);
  for (int idx = 0; idx < numCells; ++idx){
    cells.push_back(nullptr);
    cellCounts.push_back(0);
  }

  for(GeometryPtr geometry : *this){
    BoundingBoxPtr geometryBounds = geometry->getBoundingBox();
    int minCellX, minCellY, minCellZ, maxCellX, maxCellY, maxCellZ;
    getCellContainingPoint(geometryBounds->getLowerBound(), &minCellX, &minCellY, &minCellZ);
    getCellContainingPoint(geometryBounds->getUpperBound(), &maxCellX, &maxCellY, &maxCellZ);
    addGeometryToCell(geometry, minCellX, minCellY, minCellZ, maxCellX, maxCellY, maxCellZ);
  }
}

bool RegularGrid::hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const {
  float gridHitTValue;
  if(boundingBox->hitTest(ray, &gridHitTValue)){

    //  find which cell to start in
    int cellX, cellY, cellZ;
    if(boundingBox->isPointWithinBounds(ray->origin)){
      getCellContainingPoint(ray->origin, &cellX, &cellY, &cellZ);
    }
    else{
      getCellContainingPoint(ray->getPosition(gridHitTValue), &cellX, &cellY, &cellZ);
    }

    //  calculate the cell t offset and next values, and cell increments
    float tXOffset = 0, tYOffset = 0, tZOffset = 0;
    float tXNext = FLT_MAX, tYNext = FLT_MAX, tZNext = FLT_MAX;
    int cellXInc = 0, cellYInc = 0, cellZInc = 0;

    if (ray->direction.x != 0){
      float tXLB = (boundingBox->getLowerBound().x - ray->origin.x) / ray->direction.x;
      float tXUB = (boundingBox->getUpperBound().x - ray->origin.x) / ray->direction.x;
      cellXInc = ray->direction.x >= 0 ? 1 : -1;
      tXOffset = (float)fabs(tXLB - tXUB) / numCellsX;
      if (tXLB < tXUB)
        tXNext = tXLB + tXOffset * (cellX + 1);
      else
        tXNext = tXUB + tXOffset * (numCellsX - cellX);
    }
    if (ray->direction.y != 0) {
      float tYLB = (boundingBox->getLowerBound().y - ray->origin.y) / ray->direction.y;
      float tYUB = (boundingBox->getUpperBound().y - ray->origin.y) / ray->direction.y;
      cellYInc = ray->direction.y >= 0 ? 1 : -1;
      tYOffset = (float) fabs(tYLB - tYUB) / numCellsY;
      if (tYLB < tYUB)
        tYNext = tYLB + tYOffset * (cellY + 1);
      else
        tYNext = tYUB + tYOffset * (numCellsY - cellY);
    }
    if (ray->direction.z != 0){
      float tZLB = (boundingBox->getLowerBound().z - ray->origin.z) / ray->direction.z;
      float tZUB = (boundingBox->getUpperBound().z - ray->origin.z) / ray->direction.z;
      cellZInc = ray->direction.z >= 0 ? 1 : -1;
      tZOffset = (float)fabs(tZLB - tZUB) / numCellsZ;
      if (tZLB < tZUB)
        tZNext = tZLB + tZOffset * (cellZ + 1);
      else
        tZNext = tZUB + tZOffset * (numCellsZ - cellZ);
    }

    //  traverse through the cells that the ray goes through, and hit test the objects in them
    GeometryPtr cell;
    SurfaceInfo testSurfaceInfo;
    float nextTValue;   // the t value of the ray, when it's exiting the current cell
    while (true) {
      nextTValue = std::min(tXNext, std::min(tYNext, tZNext));
      cell = cells[getCellIndex(cellX, cellY, cellZ)];

      //  hit test the objects in the cell
      float testTValue;
      bool gotHit;
      if(surfaceInfo)
        gotHit = cell && cell->hitTest(ray, &testTValue, &testSurfaceInfo);
      else
        gotHit = cell && cell->hitTest(ray, &testTValue);

      //  check that the hit point is within the cell
      if (gotHit && testTValue < nextTValue){
        if (surfaceInfo){
          *surfaceInfo = testSurfaceInfo;
          if (material)
            surfaceInfo->material = material;
        }
        *hitTValue = testTValue;
        return true;
      }

      //  work out which cell is next
      if (tXNext < tYNext && tXNext < tZNext) {
        cellX += cellXInc;
        tXNext += tXOffset;
      }
      else {
        if (tYNext < tZNext){
          cellY += cellYInc;
          tYNext += tYOffset;
        }
        else {
          cellZ += cellZInc;
          tZNext += tZOffset;
        }
      }

      //  abort if we've left the grid
      if (cellX < 0 || cellY < 0 || cellZ < 0 || cellX >= numCellsX || cellY >= numCellsY || cellZ >= numCellsZ) {
        return false;
      }
    }
  }
  return false;
}

bool RegularGrid::hitTest(const Ray* ray, float* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

void RegularGrid::addGeometryToCell(GeometryPtr geometry, int minCellX, int minCellY, int minCellZ, int maxCellX, int maxCellY, int maxCellZ) {
  for(int x = minCellX; x <= maxCellX; ++x){
    for(int y = minCellY; y <= maxCellY; ++y){
      for(int z = minCellZ; z <= maxCellZ; ++z){
        int index = getCellIndex(x, y, z);
        uint cellCount = cellCounts[index];

        if (cellCount == 0){
          cells[index] = geometry;
        }
        else if (cellCount == 1){
          GeometrySet* set = new GeometrySet();
          set->push_back(cells[index]);
          set->push_back(geometry);
          cells[index].reset((Geometry*)set);
        }
        else {
          GeometrySet* set = dynamic_cast<GeometrySet*>(cells[index].get());
          ASSERT(set, "Error in Regular Grid setup! ");
          set->push_back(geometry);
        }
        cellCounts[index] += 1;
      }
    }
  }
}

uint RegularGrid::getCellIndex(int x, int y, int z) const {
  return x + y * numCellsX + z * numCellsX * numCellsY;
}
