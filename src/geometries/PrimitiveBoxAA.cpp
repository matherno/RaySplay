//
// Created by matherno on 10/09/17.
//

#include "PrimitiveBoxAA.h"

PrimitiveBoxAA::PrimitiveBoxAA() {
  boundingBox = std::make_shared<BoundingBox>();
}

PrimitiveBoxAA::PrimitiveBoxAA(const Vector3D& lowerBound, const Vector3D& upperBound) {
  boundingBox = std::make_shared<BoundingBox>();
  boundingBox->setBounds(lowerBound, upperBound);
}

void PrimitiveBoxAA::setBounds(const Vector3D& lowerBound, const Vector3D& upperBound) {
  boundingBox->setBounds(lowerBound, upperBound);
}

void PrimitiveBoxAA::setBounds(const BoundingBoxPtr boundingBox) {
  boundingBox->setBounds(boundingBox->getLowerBound(), boundingBox->getUpperBound());
}

bool PrimitiveBoxAA::hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const {
  FaceID hitFaceID;
  if(boundingBox->hitTest(ray, hitTValue, &hitFaceID)){
    if (*hitTValue > EPSILON) {
      if (surfaceInfo) {
        surfaceInfo->normal = boundingBox->getFaceNormal(hitFaceID);
        surfaceInfo->position = ray->getPosition(*hitTValue);
        surfaceInfo->material = material;
      }
      return true;
    }
  }
  return false;
}

bool PrimitiveBoxAA::hitTest(const Ray* ray, double* hitTValue) const {
  return hitTest(ray, hitTValue, nullptr);
}

void PrimitiveBoxAA::constructBoundingBox() {

}

string PrimitiveBoxAA::constructGLSLHitTest() const
  {
  return boundingBox->constructGLSLHitTest();
  }
