#include <RayTracer.h>
#include "EnvironmentLight.h"

EnvironmentLight::EnvironmentLight()
  {
  generateSamples();
  }

EnvironmentLight::EnvironmentLight(const Vector3D& colour)
  : colour(colour)
  {
  generateSamples();
  }

Vector3D EnvironmentLight::getColour() const
  {
  return colour;
  }

void EnvironmentLight::setColour(const Vector3D& colour)
  {
  this->colour = colour;
  }

void EnvironmentLight::generateSamples()
  {
  BlueNoiseSampler* sampler = new BlueNoiseSampler(10);
  sampler->generateUnitHemisphereSamples(0);
  samplerHelper.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitHemisphereMap);
  }

bool EnvironmentLight::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  Vector3D w = surfaceInfo->normal;
  Vector3D v = mathernogl::crossProduct(w, Vector3D(0.0072, 1.0, 0.0034)).getUniform();
  Vector3D u = mathernogl::crossProduct(v, w);

  Vector3D sample = samplerHelper.getNextSample();
  Vector3D direction = u*sample.x + v*sample.y + w*sample.z;

  Ray shadowRay = Ray::create(surfaceInfo->position, direction);
  double hitTValue;
  if(!RayTracer::traceShadowRay(&shadowRay, sceneDef, &hitTValue))
    {
    if (lightIntensity)
      *lightIntensity = colour;
    if (lightDirection)
      *lightDirection = direction;
    return true;
    }
  return true;
  }
