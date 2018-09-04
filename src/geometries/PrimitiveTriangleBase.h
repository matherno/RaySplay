#pragma once
//
// Created by matherno on 3/09/17.
//


#include <Geometry.h>

class PrimitiveTriangleBase : public Geometry {
public:
  virtual bool hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, double* hitTValue) const override;
  virtual void constructBoundingBox() override;

  virtual void getCorners(Vector3D* cornerA, Vector3D* cornerB, Vector3D* cornerC) const = 0;

  //  beta and gamma being barycentric coordinates on the triangle, with cornerA being the origin,
  //    cornerA->cornerB being the beta axis
  //    cornerA->cornerC being the gamma axis
  virtual Vector3D getNormal(float beta, float gamma) const = 0;
  virtual Vector2D getTexCoord(float beta, float gamma) const = 0;
};
