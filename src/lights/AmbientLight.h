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
  bool isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  Vector3D lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  Vector3D lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
