//
// Created by matt on 13/08/18.
//

#include "EmissionShader.h"

EmissionShader::EmissionShader(const Vector3D& colour) : colour(colour)
  {

  }

Vector3D EmissionShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  if (mathernogl::dotProduct(hitRay->direction, surfaceInfo->normal) < 0)
    return colour;
  return Vector3D();
  }

Vector3D EmissionShader::getSimplifiedSurfaceColour() const
  {
  return colour;
  }
