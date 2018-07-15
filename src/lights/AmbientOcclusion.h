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
  std::unique_ptr<SampleGenerator> sampleGenerator;
  float sampleRadius;
  uint sqrtNumSamples = 2;

public:
  AmbientOcclusion();
  AmbientOcclusion(const Vector3D& colour, float sampleRadius, uint sqrtNumSamples = 2);

  Vector3D getAmbientColour() const;
  void setAmbientColour(const Vector3D& ambientColour);
  float getSampleRadius() const;
  void setSampleRadius(float sampleRadius);
  void generateSamples(uint sqrtNumSamples);
  uint getSqrtNumSamples() const { return sqrtNumSamples; }

  virtual bool isAmbient() override { return true; }
  virtual bool isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual Vector3D lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
  virtual Vector3D lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) override;
};
