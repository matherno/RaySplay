#pragma once
//
// Created by matherno on 3/09/17.
//


#include <RaySplayConst.h>
#include "PrimitiveTriangleBase.h"

class PrimitiveTriangle : public PrimitiveTriangleBase {
  Vector3D cornerA, cornerB, cornerC;
  Vector3D normal;

public:
  PrimitiveTriangle();
  PrimitiveTriangle(const Vector3D& cornerA, const Vector3D& cornerB, const Vector3D& cornerC);

  void setCorners(const Vector3D& cornerA, const Vector3D& cornerB, const Vector3D& cornerC);
  Vector3D getNormal() const { return normal; }

  virtual void getCorners(Vector3D* cornerA, Vector3D* cornerB, Vector3D* cornerC) const override;
  virtual Vector3D getNormal(float beta, float gamma) const override;
};
