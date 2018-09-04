#pragma once
//
// Created by matherno on 31/07/17.
//

#include <mathernogl/MathernoGL.h>
#include "Geometry.h"

class PrimitivePlane : public Geometry {
private:
  mathernogl::Vector3D point;
  mathernogl::Vector3D normal;

public:
  PrimitivePlane();
  PrimitivePlane(const mathernogl::Vector3D& point, const mathernogl::Vector3D& normal);
  virtual ~PrimitivePlane() {};

  const mathernogl::Vector3D& getPoint() const;
  const mathernogl::Vector3D& getNormal() const;
  void setPoint(const mathernogl::Vector3D& point);
  void setPoint(float x, float y, float z);
  void setNormal(const mathernogl::Vector3D& normal);
  void setNormal(float x, float y, float z);

  virtual bool hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, double* hitTValue) const override;
  virtual void constructBoundingBox() override;
  virtual const BoundingBoxPtr getBoundingBox() const override;
};
