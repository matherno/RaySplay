#pragma once
//
// Created by matherno on 10/09/17.
//


#include <Geometry.h>

class GeometrySet : public Geometry, public std::vector<GeometryPtr> {
public:
  GeometrySet(){};
  virtual ~GeometrySet(){};

  virtual bool hitTest(const Ray* ray, double* hitTValue, SurfaceInfo* surfaceInfo) const override;
  virtual bool hitTest(const Ray* ray, double* hitTValue) const override;
  virtual void constructBoundingBox() override;

  virtual bool init() override;

  GeometryPtr getGeometry(int index) const { return operator[]((size_t)index); }
};
