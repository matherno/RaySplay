//
// Created by matherno on 21/08/17.
//

#include <RayTracer.h>
#include <Samplers.h>
#include "ReflectionShader.h"

ReflectionShader::ReflectionShader() : PhongShader() {
  initSampler();
}

ReflectionShader::ReflectionShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, Vector3D mirrorCol)
    : PhongShader(diffuseColour, specularColour, specularExp, true), mirrorCol(mirrorCol) {
  initSampler();
}

ReflectionShader::ReflectionShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp, Vector3D mirrorCol)
    : PhongShader(diffuseTexture, specularColour, specularExp, true), mirrorCol(mirrorCol) {
  initSampler();
}

void ReflectionShader::initSampler(uint sqrtNumSamples) {
  sampler.reset(new BlueNoiseSampler(sqrtNumSamples));
}

void ReflectionShader::setMirrorCol(const Vector3D& mirrorCol) {
  this->mirrorCol = mirrorCol;
}

void ReflectionShader::setFuzziness(float fuzziness) {
  if (fuzziness < 1e-4) {
    isPerfectReflection = true;
  }
  else {
    uint sqrtNumSamples = 1;
    if (fuzziness > 0.2)
      sqrtNumSamples = 2;
    initSampler(sqrtNumSamples);

    float reflSamplingExp = mathernogl::clampf(1.0f - fuzziness, 0.0f, 1.0f ) * 5.0f;
    reflSamplingExp = (float)pow(10.0, reflSamplingExp);
    sampler->generateUnitHemisphereSamples(reflSamplingExp);
    isPerfectReflection = false;
  }
}


Vector3D ReflectionShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  Vector3D phong = PhongShader::shadeSurface(hitRay, surfaceInfo, sceneDef);
  Vector3D reflectedLight = calcReflectedLight(hitRay, surfaceInfo, sceneDef);
  return phong + mirrorCol * reflectedLight;
}

using namespace mathernogl;

Vector3D ReflectionShader::calcReflectedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  if (isPerfectReflection) {
    Ray reflectedRay = Ray::create(surfaceInfo->position, reflect(hitRay->direction, surfaceInfo->normal));
    reflectedRay.depth = hitRay->depth + 1;
    Vector3D reflectedCol;
    if(RayTracer::traceRay(&reflectedRay, sceneDef, &reflectedCol))
      return reflectedCol;
    else
      return sceneDef->bgColour;
  }
  else {
    //  construct orthonormal basis
    Vector3D w = reflect(hitRay->direction, surfaceInfo->normal);
    Vector3D u = crossProduct(Vector3D(0.0023, 1, 0.0048), w);
    u.makeUniform();
    Vector3D v = crossProduct(u, w);

    //  use all the samples in the sample set to obtain a resultant light colour value for this surface
    SampleSetPtr sampleSet = sampler->getSampleSet(SampleGenerator::unitHemisphereMap);
    Vector3D sample, resultingLight(0);

    while(sampleSet->nextSample(&sample)) {
      //  use orthonormal basis to transform the sample hemisphere into the direction of the perfect reflection
      Vector3D sampleReflection = (u * sample.x) + (v * sample.y) + (w * sample.z);
      if (dotProduct(sampleReflection, surfaceInfo->normal) < 0.0)
        sampleReflection = (u * sample.x * -1.0f) + (v * sample.y * -1.0f) + (w * sample.z);

      //  trace a reflected ray to determine the colour of the light reaching the surface from that direction
      Ray reflectedRay = Ray::create(surfaceInfo->position, sampleReflection);
      reflectedRay.depth = hitRay->depth + 1;
      Vector3D reflectedCol(0);
      if(RayTracer::traceRay(&reflectedRay, sceneDef, &reflectedCol))
        resultingLight += reflectedCol;
      else
        resultingLight += sceneDef->bgColour;
    }
    return resultingLight / sampler->numSamples;
  }

}

