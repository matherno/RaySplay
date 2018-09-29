#pragma once
//
// Created by matherno on 9/08/17.
//


#include <Shader.h>

class FlatColourShader : public Shader {
private:
  mathernogl::Vector3D colour;

public:
  FlatColourShader(float r, float g, float b);

  const mathernogl::Vector3D& getColour() const;
  void setColour(const mathernogl::Vector3D& colour);
  void setColour(float r, float g, float b);

  mathernogl::Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual Vector3D getSimplifiedSurfaceColour() const override;
  };
