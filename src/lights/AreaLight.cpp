//
// Created by matt on 8/08/18.
//

#include <RayTracer.h>
#include "AreaLight.h"


AreaLight::AreaLight(GeometryPtr lightGeometry) : geometry(lightGeometry)
  {
  }

bool AreaLight::startShadingAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  Vector3D currentSamplePos;
  if(geometry->getSurfaceSample(&currentSamplePos, &currentSampleNorm))
    {
    setPosition(currentSamplePos);
    return true;
    }
  return false;
  }

bool AreaLight::isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  const bool canCastShadows = geometry->canCastShadows();
  geometry->setCastShadows(false);
  const bool result = PointLight::isPointInShadow(surfaceInfo, sceneDef);
  geometry->setCastShadows(canCastShadows);
  return result;
  }

Vector3D AreaLight::lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  Vector3D intensity = PointLight::lightIntensityAtPoint(surfaceInfo, sceneDef);
  intensity *= std::max(mathernogl::dotProduct(surfaceInfo->normal, currentSampleNorm), 0.0);
  return intensity;
  }

