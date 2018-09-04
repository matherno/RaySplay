//
// Created by matherno on 10/09/17.
//

#include <RayTracer.h>
#include "GeometrySet.h"


bool GeometrySet::init() {
  if (!isInitialised){
    for (GeometryPtr geometry : *this) {
      geometry->init();
    }
  }
  return Geometry::init();
}

void GeometrySet::constructBoundingBox() {
  boundingBox = std::shared_ptr<BoundingBox>(new BoundingBox());
  for (GeometryPtr geometry : *this) {
    boundingBox->addBoundingBox(geometry->getBoundingBox());
  }
  boundingBox->expandBounds();
}

bool GeometrySet::hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const {
  bool gotHit = false;
  double testTValue;
  SurfaceInfo testSurfaceInfo;

  for (GeometryPtr geometry : *this) {
    if (geometry->hitTest(ray, &testTValue, &testSurfaceInfo)) {
      if (!gotHit || testTValue < *hitTValue) {
        *hitTValue = testTValue;
        *surfaceInfo = testSurfaceInfo;
        gotHit = true;
      }
    }
  }

  if(gotHit && material && surfaceInfo){
    surfaceInfo->material = material;
  }
  return gotHit;
}

bool GeometrySet::hitTest(const Ray* ray, double* hitTValue) const {
  bool gotHit = false;
  double testTValue;
  for (GeometryPtr geometry : *this) {
    if (geometry->hitTest(ray, &testTValue)) {
      if (!gotHit || testTValue < *hitTValue) {
        *hitTValue = testTValue;
        gotHit = true;
      }
    }
  }
  return gotHit;
}
