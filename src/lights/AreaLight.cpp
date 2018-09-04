//
// Created by matt on 8/08/18.
//

#include <RayTracer.h>
#include "AreaLight.h"


AreaLight::AreaLight(GeometryPtr lightGeometry) : geometry(lightGeometry)
  {
  }

bool AreaLight::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  Vector3D currentSamplePos, currentSampleNorm;
  if(geometry->getSurfaceSample(&currentSamplePos, &currentSampleNorm))
    {
    const bool canCastShadows = geometry->canCastShadows();
    geometry->setCastShadows(false);
    bool res = PointLight::lightAtSurface(surfaceInfo, sceneDef, currentSamplePos, lightIntensity, lightDirection);
    geometry->setCastShadows(canCastShadows);
    return res;
    }
  return false;
  }
