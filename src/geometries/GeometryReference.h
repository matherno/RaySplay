#pragma once
//
// Created by matherno on 1/10/17.
//


#include <Geometry.h>
#include "mathernogl/MathernoGL.h"

class GeometryReference : public Geometry {
private:
  GeometryPtr geometry;
  mathernogl::Transform transform;
  mathernogl::Transform invTransform;

public:
  GeometryReference(const GeometryPtr& geometry);
  virtual ~GeometryReference() {};

  GeometryPtr getGeometry() { return geometry; };
  const mathernogl::Transform* getTransform() const { return &transform; };
  const mathernogl::Transform* getInvTransform() const { return &invTransform; };
  void setTransform(const mathernogl::Transform& transform);
  virtual ShaderPtr getMaterial() const override;

  virtual bool init() override;
  virtual bool hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, double* hitTValue) const override;
  virtual void constructBoundingBox() override;
  virtual string constructGLSLHitTest() const override;

  };
