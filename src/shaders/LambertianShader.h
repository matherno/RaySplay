#pragma once
//
// Created by matherno on 10/08/17.
//


#include <Shader.h>
#include <Texture.h>

class LambertianShader : public Shader {
  Vector3D diffuseColour;
  TexturePtr diffuseTexture;

public:
  LambertianShader();
  LambertianShader(float r, float g, float b);
  LambertianShader(const Vector3D& diffuseColour);
  LambertianShader(TexturePtr diffuseTexture);

  Vector3D getDiffuseColour() const;
  void setDiffuseColour(const Vector3D& diffuseColour);
  void setDiffuseTexture(TexturePtr diffuseTexture);

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;

private:
  inline Vector3D getDiffuseColour(const SurfaceInfo* surfaceInfo) const;
};
