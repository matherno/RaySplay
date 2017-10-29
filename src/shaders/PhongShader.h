#pragma once
//
// Created by matherno on 12/08/17.
//


#include <Shader.h>

class PhongShader : public Shader {
private:
  Vector3D diffuseColour;
  Vector3D specularColour;
  float specularExp = 25;
  bool blinnPhong = true;

public:
  PhongShader();
  PhongShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, bool blinnPhong = true);

  Vector3D getDiffuseColour() const;
  Vector3D getSpecularColour() const;
  float getSpecularExp() const;

  void setDiffuseColour(const Vector3D& diffuseColour);
  void setSpecularColour(const Vector3D& specularColour);
  void setSpecularExp(float specularExp);

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
