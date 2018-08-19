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
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir) = 0;

  /*
   *  obtains a random sample of incoming light direction, when looking at a surface from a given view direction
   *  directions are described from surface point
   */
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir) = 0;
  };


class LightScatterDiffuse : public LightScatterBase
  {
private:
  ContinousSamplerHelper incomingLightSampler;

public:
  LightScatterDiffuse();
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir) override;
  double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& lightDir);
  };


class LightScatterSpecular : public LightScatterBase
  {
private:
  double cosineExp = 0;
  ContinousSamplerHelper incomingLightSampler;

public:
  LightScatterSpecular(double cosineExp);
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir) override;
  };


class LightScatterPerfectSpecular : public LightScatterBase
  {
public:
  virtual double getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir) override;
  virtual Vector3D sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir) override;
  };