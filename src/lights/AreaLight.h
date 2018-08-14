#pragma once

#include "PointLight.h"

/*
*   
*/

class AreaLight : public PointLight
  {
private:
  GeometryPtr geometry;
  Vector3D currentSampleNorm;

public:
  AreaLight(GeometryPtr lightGeometry);

  virtual bool startShadingAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  Vector3D lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual bool isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  };
