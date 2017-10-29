#pragma once
//
// Created by matherno on 1/08/17.
//

#include <mathernogl/MathernoGL.h>
#include "Geometry.h"


class PrimitiveSphere : public Geometry {
private:
  mathernogl::Vector3D centre;
  float radius;
  float radiusSquared;

public:
  PrimitiveSphere();
  PrimitiveSphere(const mathernogl::Vector3D& centre, float radius);

  void setCentre(const mathernogl::Vector3D& centre);
  void setCentre(float x, float y, float z);
  void setRadius(float radius);

  const mathernogl::Vector3D& getCentre() const;
  float getRadius() const;
  float getRadiusSquared() const;

  virtual bool hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, float* hitTValue) const override;
  virtual void constructBoundingBox() override;
};
