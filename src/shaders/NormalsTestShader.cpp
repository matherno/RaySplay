//
// Created by matherno on 8/11/17.
//

#include "NormalsTestShader.h"

Vector3D NormalsTestShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  return (surfaceInfo->normal * 0.5f) + 0.5f;
}
