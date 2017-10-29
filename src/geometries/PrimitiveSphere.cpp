//
// Created by matherno on 1/08/17.
//

#include "PrimitiveSphere.h"
#include "RaySplayConst.h"

PrimitiveSphere::PrimitiveSphere() : centre(0), radius(1) {
  radiusSquared = radius * radius;
}

PrimitiveSphere::PrimitiveSphere(const mathernogl::Vector3D& centre, float radius) : centre(centre), radius(radius) {
  radiusSquared = radius * radius;
}

const mathernogl::Vector3D& PrimitiveSphere::getCentre() const {
  return centre;
}

void PrimitiveSphere::setCentre(const mathernogl::Vector3D& centre) {
  this->centre = centre;
}

void PrimitiveSphere::setCentre(float x, float y, float z) {
  this->centre.x = x;
  this->centre.y = y;
  this->centre.z = z;
}

float PrimitiveSphere::getRadius() const {
  return radius;
}

void PrimitiveSphere::setRadius(float radius) {
  this->radius = radius;

  radiusSquared = radius * radius;
}

float PrimitiveSphere::getRadiusSquared() const {
  return radiusSquared;
}

bool PrimitiveSphere::hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const {
  using namespace mathernogl;
  Vector3D originToCentre = ray->origin - centre;
  float a = dotProduct(ray->direction, ray->direction);
  float b = 2.0f * dotProduct(originToCentre, ray->direction);
  float c = dotProduct(originToCentre, originToCentre) - radiusSquared;
  float discriminant = b*b - (4.0f*a*c);

  if(discriminant > 0){
    float sqrtPart = (float)sqrt(discriminant);
    float denominator = 2*a;

    float tValueResult = (-b - sqrtPart)/denominator;
    if(tValueResult > EPSILON){
      *hitTValue = tValueResult;
      if(surfaceInfo){
        surfaceInfo->position = ray->getPosition(tValueResult);
        surfaceInfo->normal = (surfaceInfo->position - centre) / radius;
        surfaceInfo->material = material;
      }
      return true;
    }


    tValueResult = (-b + sqrtPart)/denominator;
    if(tValueResult > EPSILON){
      *hitTValue = tValueResult;
      if(surfaceInfo){
        surfaceInfo->position = ray->getPosition(tValueResult);
        surfaceInfo->normal = (surfaceInfo->position - centre) / radius;
        surfaceInfo->material = material;
      }
      return true;
    }
  }
  return false;
}

bool PrimitiveSphere::hitTest(const Ray* ray, float* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

void PrimitiveSphere::constructBoundingBox() {
  boundingBox = std::make_shared<BoundingBox>(centre - radius, centre + radius);
}



