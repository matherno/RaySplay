//
// Created by matherno on 31/07/17.
//

#include "PrimitivePlane.h"
#include "RaySplayConst.h"

PrimitivePlane::PrimitivePlane() : point(0,0,0), normal(0,1,0){

}

PrimitivePlane::PrimitivePlane(const mathernogl::Vector3D& point, const mathernogl::Vector3D& normal) : point(point), normal(normal) {

}

bool PrimitivePlane::hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const {
  using namespace mathernogl;
  float tValue = dotProduct((point - ray->origin), normal) / dotProduct(ray->direction, normal);
  if(tValue > EPSILON){
    *hitTValue = tValue;
    if(surfaceInfo){
      surfaceInfo->position = ray->getPosition(tValue);
      surfaceInfo->normal = normal;
      surfaceInfo->material = material;
    }
    return true;
  }
  return false;
}

bool PrimitivePlane::hitTest(const Ray* ray, float* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

const mathernogl::Vector3D& PrimitivePlane::getPoint() const {
  return point;
}

void PrimitivePlane::setPoint(const mathernogl::Vector3D& point) {
  this->point = point;
}

const mathernogl::Vector3D& PrimitivePlane::getNormal() const {
  return normal;
}

void PrimitivePlane::setNormal(const mathernogl::Vector3D& normal) {
  this->normal = normal.getUniform();
}

void PrimitivePlane::setPoint(float x, float y, float z) {
  this->point.x = x;
  this->point.y = y;
  this->point.z = z;
}

void PrimitivePlane::setNormal(float x, float y, float z) {
  this->normal.x = x;
  this->normal.y = y;
  this->normal.z = z;
  this->normal.makeUniform();
}

const BoundingBoxPtr PrimitivePlane::getBoundingBox() const {
  mathernogl::logWarning("Can not obtain a bounding box of an infinite plane! ");
  return Geometry::getBoundingBox();
}

void PrimitivePlane::constructBoundingBox() {
  boundingBox = std::shared_ptr<BoundingBox>(new BoundingBox());
}



