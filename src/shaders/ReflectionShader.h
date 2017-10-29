#pragma once
//
// Created by matherno on 21/08/17.
//


#include "PhongShader.h"

class ReflectionShader : public PhongShader {
public:
  ReflectionShader();
  ReflectionShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, bool blinnPhong = true);

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
