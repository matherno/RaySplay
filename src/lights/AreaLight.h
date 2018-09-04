#pragma once

#include "PointLight.h"

/*
*   
*/

class AreaLight : public PointLight
  {
private:
  GeometryPtr geometry;

public:
  AreaLight(GeometryPtr lightGeometry);
  virtual bool lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection) override;
  };
