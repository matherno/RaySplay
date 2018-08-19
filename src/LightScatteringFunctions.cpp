#include "LightScatteringFunctions.h"
#include "Samplers.h"

/*
 *  LightScatterDiffuse
 */

LightScatterDiffuse::LightScatterDiffuse()
  {
  BlueNoiseSampler* sampler = new BlueNoiseSampler();
  sampler->generateUnitHemisphereSamples(0);
  incomingLightSampler.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitHemisphereMap);
  }

double LightScatterDiffuse::getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir)
  {
  return std::max(0.0, mathernogl::dotProduct(surfaceNormal, lightDir));
  }

Vector3D LightScatterDiffuse::sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir)
  {
  const Vector3D& w = surfaceNormal;
  Vector3D u = crossProduct(Vector3D(0.0023, 1, 0.0048), w);
  u.makeUniform();
  Vector3D v = crossProduct(u, w);
  const Vector3D sample = incomingLightSampler.getNextSample();
  return (u * sample.x) + (v * sample.y) + (w * sample.z);
  }

double LightScatterDiffuse::getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& lightDir)
  {
  //  just for convenience as radiance is independent from view direction for diffuse
  return getSurfaceRadianceFactor(surfaceNormal, Vector3D(), lightDir);
  }


/*
 *  LightScatterSpecular
 */

LightScatterSpecular::LightScatterSpecular(double cosineExp)
  {
  this->cosineExp = cosineExp;
  BlueNoiseSampler* sampler = new BlueNoiseSampler();
  sampler->generateUnitHemisphereSamples(cosineExp);
  incomingLightSampler.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitHemisphereMap);
  }

double LightScatterSpecular::getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir)
  {
  if (mathernogl::dotProduct(surfaceNormal, lightDir) < 0)
    return 0;

  Vector3D halfVector = lightDir + viewDir;
  halfVector.makeUniform();
  return pow(mathernogl::dotProduct(halfVector, surfaceNormal), cosineExp);
  }

Vector3D LightScatterSpecular::sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir)
  {
  const Vector3D reflected = mathernogl::reflect(viewDir * -1, surfaceNormal);
  const Vector3D& w = reflected;
  Vector3D u = crossProduct(Vector3D(0.0023, 1, 0.0048), w);
  u.makeUniform();
  Vector3D v = crossProduct(u, w);

  Vector3D sample = incomingLightSampler.getNextSample();
  sample = (u * sample.x) + (v * sample.y) + (w * sample.z);
  if (mathernogl::dotProduct(sample, surfaceNormal) < 0.0)
    sample = (u * sample.x * -1.0f) + (v * sample.y * -1.0f) + (w * sample.z);
  return sample;
  }


/*
 *  LightScatterPerfectSpecular (LightScatterSpecular with infinite cosineExp)
 */

double LightScatterPerfectSpecular::getSurfaceRadianceFactor(const Vector3D& surfaceNormal, const Vector3D& viewDir, const Vector3D& lightDir)
  {
  return 1;
  }

Vector3D LightScatterPerfectSpecular::sampleIncomingLightDir(const Vector3D& surfaceNormal, const Vector3D& viewDir)
  {
  return mathernogl::reflect(viewDir * -1, surfaceNormal);
  }
