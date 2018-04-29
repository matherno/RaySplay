#pragma once
//
// Created by matherno on 31/07/17.
//

#include <memory>

class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;

#include "Ray.h"
#include "SurfaceInfo.h"
#include "Shader.h"
#include "BoundingBox.h"

class Geometry {
protected:
  ShaderPtr material;
  BoundingBoxPtr boundingBox;
  bool isInitialised = false;

public:
  Geometry() {};
  virtual ~Geometry() {};

  ShaderPtr getMaterial() const { return material; }
  void setMaterial(ShaderPtr material) { this->material = material; }
  virtual const BoundingBoxPtr getBoundingBox() const { return boundingBox; };

  //  called before render to initialise if needed
  //  returns true if initialisation was performed, false if it already has been
  virtual bool init(){
    if (!isInitialised){
      constructBoundingBox();
      isInitialised = true;
      return true;
    }
    return false;
  };

  //  returns true if the given ray hits this geometry, within the given t value bounds
  //  if hits, fills in surfaceInfo with the relevant information
  virtual bool hitTest(const Ray* ray, float* hitTValue, SurfaceInfo* surfaceInfo) const = 0;

  //  like hitTest but doesn't need surface properties, so should be more efficient if possible
  virtual bool hitTest(const Ray* ray, float* hitTValue) const = 0;

  //  initialise this geometry object, including constructing its bounding box
  virtual void constructBoundingBox() = 0;

};

