#pragma once

#include "PhongShader.h"
#include "ReflectionShader.h"

/*
*   
*/

class TransparencyShader : public ReflectionShader
  {
private:
  Vector3D internalColour;
  double refractionIndex = 0;
  std::unique_ptr<LightScatterBase> refractiveScatterer;

public:
  TransparencyShader();
  TransparencyShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp);
  TransparencyShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp);

  Vector3D getInternalCol() const { return internalColour; }
  void setInternalCol(const Vector3D& internalCol);
  double getRefractionIndex() const { return refractionIndex; }
  void setRefractionIndex(double idx);

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;

private:
  void setupScatterer();
  Vector3D calcRefractedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef);
  Vector3D calcReflectedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef);
  };
