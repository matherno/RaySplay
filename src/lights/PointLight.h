#pragma once
//
// Created by matherno on 12/08/17.
//


#include <LightSource.h>

class PointLight : public LightSource {
private:
  Vector3D position;
  Vector3D colour;
  float radius = 1;
  float falloffExp = 1;

public:
  PointLight();
  PointLight(const Vector3D& colour, const Vector3D& position, float radius, float falloffExp = 1);

  Vector3D getPosition() const;
  Vector3D getColour() const;
  float getRadius() const { return radius; }
  float getFallOffExp() const { return falloffExp; }
  void setPosition(const Vector3D& position);
  void setColour(const Vector3D& colour);

  virtual bool isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual Vector3D lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual Vector3D lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
