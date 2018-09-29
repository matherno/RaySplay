#pragma once
//
// Created by matherno on 17/08/17.
//


#include <LightSource.h>
#include <SampleGenerator.h>
#include <Samplers.h>

class AmbientOcclusion : public LightSource {
private:
  Vector3D ambientColour;
  ContinousSamplerHelper samplerHelper;
  float sampleRadius;

public:
  AmbientOcclusion();
  AmbientOcclusion(const Vector3D& colour, float sampleRadius);

  Vector3D getAmbientColour() const;
  void setAmbientColour(const Vector3D& ambientColour);
  float getSampleRadius() const;
  void setSampleRadius(float sampleRadius);
  void generateSamples();

  virtual bool isAmbient() override { return true; }
  virtual bool lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection) override;
  virtual string constructGLSLLightSurface() const override;
  };
