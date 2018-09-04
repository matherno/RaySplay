#pragma once
//
// Created by matherno on 10/08/17.
//


#include <LightSource.h>

class AmbientLight : public LightSource {
private:
  mathernogl::Vector3D colour;

public:
  AmbientLight() {}
  AmbientLight(const mathernogl::Vector3D& colour);
  AmbientLight(float r, float g, float b);

  const Vector3D getColour() const;
  void setColour(const Vector3D& colour);

  bool isAmbient() override { return true; };

  virtual bool lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection) override;
  };
