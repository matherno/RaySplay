#pragma once
//
// Created by matherno on 12/08/17.
//


#include <Shader.h>
#include <Texture.h>
#include <LightScatteringFunctions.h>

class PhongShader : public Shader {
private:
  Vector3D diffuseColour;
  Vector3D specularColour;
  float specularExp = 25;
  bool blinnPhong = true;
  TexturePtr diffuseTexture;
  std::unique_ptr<LightScatterBase> diffuseScatterer;
  std::unique_ptr<LightScatterBase> specularScatterer;

public:
  PhongShader();
  PhongShader(double diffuseR, double diffuseG, double diffuseB);
  PhongShader(const Vector3D& diffuseColour);
  PhongShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, bool blinnPhong = true);
  PhongShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp, bool blinnPhong = true);

  Vector3D getDiffuseColour() const;
  Vector3D getSpecularColour() const;
  float getSpecularExp() const;

  void setDiffuseColour(const Vector3D& diffuseColour);
  void setDiffuseTexture(TexturePtr diffuseTexture);
  void setSpecularColour(const Vector3D& specularColour);
  void setSpecularExp(float specularExp);

  //  set to use blinn-phong algorithm, rather than phong
  void setBlinnPhong(bool blinnPhong) { this->blinnPhong = blinnPhong; }

  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;

protected:
  Vector3D getDiffuseColour(const SurfaceInfo* surfaceInfo) const;
  void setupScatterers();
};
