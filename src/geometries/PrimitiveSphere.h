#pragma once
//
// Created by matherno on 1/08/17.
//

#include <mathernogl/MathernoGL.h>
#include <SampleGenerator.h>
#include "Geometry.h"


class PrimitiveSphere : public Geometry {
private:
  mathernogl::Vector3D centre;
  float radius;
  float radiusSquared;
  ContinousSamplerHelper samplerHelper;

public:
  PrimitiveSphere();
  PrimitiveSphere(const mathernogl::Vector3D& centre, float radius);

  void setCentre(const mathernogl::Vector3D& centre);
  void setCentre(float x, float y, float z);
  void setRadius(float radius);

  const mathernogl::Vector3D& getCentre() const;
  float getRadius() const;
  float getRadiusSquared() const;
  Vector2D getTextureCoord(const Vector3D& position) const;

  virtual bool hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, double* hitTValue) const override;
  virtual void constructBoundingBox() override;

  virtual bool getSurfaceSample(Vector3D* position, Vector3D* normal) override;

protected:
  void setupSurfaceSampler();
  };
