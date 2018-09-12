#pragma once

#include "RaySplayConst.h"
#include "SurfaceInfo.h"
#include "SampleGenerator.h"

class LightScatterBase
  {
public:
  /*
   *  obtains the factor of light irradiated from the given light direction, that is radiated in the given view direction
   *  directions are described from surface point
   */
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& surfaceToView, const Vector3D& surfaceToLight) = 0;

  /*
   *  obtains a random sample of incoming light direction, when looking at a surface from a given view direction
   *  directions are described from surface point
   */
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& surfaceToView) = 0;
  };


class LightScatterDiffuse : public LightScatterBase
  {
private:
  ContinousSamplerHelper incomingLightSampler;

public:
  LightScatterDiffuse();
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& surfaceToView, const Vector3D& surfaceToLight) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& surfaceToView) override;
  double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& surfaceToLight);
  };


class LightScatterSpecular : public LightScatterBase
  {
private:
  double cosineExp = 0;
  ContinousSamplerHelper incomingLightSampler;

public:
  LightScatterSpecular(double cosineExp);
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& surfaceToView, const Vector3D& surfaceToLight) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& surfaceToView) override;
  };


class LightScatterPerfectSpecular : public LightScatterBase
  {
public:
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& surfaceToView, const Vector3D& surfaceToLight) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& surfaceToView) override;
  };

class LightScatterRefraction : public LightScatterBase
  {
private:
  double refractionIdx = 1;

public:
  LightScatterRefraction(double refractionIndex) : refractionIdx(std::max(refractionIndex, 0.001)) {}
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& surfaceToView, const Vector3D& surfaceToLight) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& surfaceToView) override;
  };