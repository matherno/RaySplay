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

bool AmbientOcclusion::isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  return false;
}

Vector3D AmbientOcclusion::lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  ASSERT(false, "Requesting the direction of ambient light. Need to check isAmbient().");
  return Vector3D();
}

Vector3D AmbientOcclusion::lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  if (sceneDef->disableAmbientOcclusion)
    return ambientColour;

  Vector3D w = surfaceInfo->normal;
  Vector3D v = mathernogl::crossProduct(w, Vector3D(0.0072, 1.0, 0.0034)).getUniform();
  Vector3D u = mathernogl::crossProduct(v, w);

  Vector3D sample = samplerHelper.getNextSample();

  Ray shadowRay = Ray::create(surfaceInfo->position, u*sample.x + v*sample.y + w*sample.z);
  float hitTValue;
  double shadowIntensity = 0;
  if(RayTracer::traceShadowRay(&shadowRay, sceneDef, &hitTValue)){
    shadowIntensity = mathernogl::clampd(1.0 - (hitTValue/sampleRadius), 0.0, 1.0);
  }

  return ambientColour * (1.0f - shadowIntensity);
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
  sampler->generateUnitHemisphereSamples(1);
  samplerHelper.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitHemisphereMap);
  }
