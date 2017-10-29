#pragma once
//
// Created by matherno on 10/08/17.
//


#include <Shader.h>

class LambertianShader : public Shader {
  Vector3D diffuseColour;

public:
  LambertianShader();
  LambertianShader(float r, float g, float b);
  LambertianShader(const Vector3D& diffuseColour);

  Vector3D getDiffuseColour() const;
  void setDiffuseColour(const Vector3D& diffuseColour);

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
