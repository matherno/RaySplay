//
// Created by matherno on 17/08/17.
//

#include <RayTracer.h>
#include "AmbientOcclusion.h"

AmbientOcclusion::AmbientOcclusion(){
  generateSamples();
}

AmbientOcclusion::AmbientOcclusion(const Vector3D& colour, float sampleRadius)
    : ambientColour(colour), sampleRadius(sampleRadius) {
  generateSamples();
}

Vector3D AmbientOcclusion::getAmbientColour() const {
  return ambientColour;
}

void AmbientOcclusion::setAmbientColour(const Vector3D& ambientColour) {
  this->ambientColour = ambientColour;
}

float AmbientOcclusion::getSampleRadius() const {
  return sampleRadius;
}

void AmbientOcclusion::setSampleRadius(float sampleRadius) {
  this->sampleRadius = sampleRadius;
}

void AmbientOcclusion::generateSamples() {
  BlueNoiseSampler* sampler = new BlueNoiseSampler(10);
  sampler->generateUnitHemisphereSamples(0);
  samplerHelper.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitHemisphereMap);
  }

bool AmbientOcclusion::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  if (sceneDef->disableAmbientOcclusion)
    {
    if (lightIntensity)
      *lightIntensity = ambientColour;
    return true;
    }

  Vector3D w = surfaceInfo->normal;
  Vector3D v = mathernogl::crossProduct(w, Vector3D(0.0072, 1.0, 0.0034)).getUniform();
  Vector3D u = mathernogl::crossProduct(v, w);

  Vector3D sample = samplerHelper.getNextSample();

  Ray shadowRay = Ray::create(surfaceInfo->position, u*sample.x + v*sample.y + w*sample.z);
  double hitTValue;
  double shadowIntensity = 0;
  if(RayTracer::traceShadowRay(&shadowRay, sceneDef, &hitTValue))
    {
    shadowIntensity = mathernogl::clampd(hitTValue/sampleRadius, 0.0, 1.0);
    shadowIntensity *= shadowIntensity;
    shadowIntensity = (1.0 - shadowIntensity);
    }

  if (lightIntensity)
    *lightIntensity = ambientColour * (1.0f - shadowIntensity);
  return true;
  }

string AmbientOcclusion::constructGLSLLightSurface() const
  {
  return "lightCol = " + glslVec3(ambientColour) + ";";
  }
