#pragma once
//
// Created by matherno on 10/09/17.
//


#include <Geometry.h>
#include <BoundingBox.h>

class PrimitiveBoxAA : public Geometry {
public:
  PrimitiveBoxAA();
  PrimitiveBoxAA(const Vector3D& lowerBound, const Vector3D& upperBound);

  Vector3D getLowerBound() const { return boundingBox->getLowerBound(); }
  Vector3D getUpperBound() const { return boundingBox->getUpperBound(); }
  void setBounds(const Vector3D& lowerBound, const Vector3D& upperBound);
  void setBounds(const BoundingBoxPtr boundingBox);

  virtual bool hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, double* hitTValue) const override;
  virtual void constructBoundingBox() override;
  virtual string constructGLSLHitTest() const override;
  };
