//
// Created by matherno on 3/09/17.
//

#include "PrimitiveTriangle.h"

PrimitiveTriangle::PrimitiveTriangle() {}

PrimitiveTriangle::PrimitiveTriangle(const Vector3D& cornerA, const Vector3D& cornerB, const Vector3D& cornerC) {
  setCorners(cornerA, cornerB, cornerC);
}

void PrimitiveTriangle::setCorners(const Vector3D& cornerA, const Vector3D& cornerB, const Vector3D& cornerC) {
  this->cornerA = cornerA;
  this->cornerB = cornerB;
  this->cornerC = cornerC;
  normal = mathernogl::crossProduct(cornerC - cornerB, cornerA - cornerB).getUniform();
}

void PrimitiveTriangle::getCorners(Vector3D* cornerA, Vector3D* cornerB, Vector3D* cornerC) const {
  *cornerA = this->cornerA;
  *cornerB = this->cornerB;
  *cornerC = this->cornerC;
}

Vector3D PrimitiveTriangle::getNormal(float beta, float gamma) const {
  return normal;
}


