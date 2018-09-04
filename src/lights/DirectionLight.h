#pragma once
//
// Created by matherno on 10/08/17.
//


#include <LightSource.h>

class DirectionLight : public LightSource {
  Vector3D colour;
  Vector3D direction;
  Vector3D directionFlipped;  // direction that the light is coming from at a surface

public:
  DirectionLight();
  DirectionLight(const Vector3D& colour, const Vector3D& direction);

  const Vector3D getColour() const;
  void setColour(const Vector3D& colour);
  const Vector3D getDirection() const;
  void setDirection(const Vector3D& direction);

  virtual bool lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection) override;
  };
