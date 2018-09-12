#pragma once

#include <LightSource.h>
#include <SampleGenerator.h>
#include <Samplers.h>

class EnvironmentLight : public LightSource
  {
private:
  Vector3D colour;
  ContinousSamplerHelper samplerHelper;

public:
  EnvironmentLight();
  EnvironmentLight(const Vector3D& colour);

  Vector3D getColour() const;
  void setColour(const Vector3D& colour);
  void generateSamples();

  virtual bool lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection) override;
  };
