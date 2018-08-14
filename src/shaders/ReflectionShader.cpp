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
  BlueNoiseSampler* sampler = new BlueNoiseSampler(sqrtNumSamples);
  sampler->generateUnitHemisphereSamples(fuzziness > 0 ? 1.0f / fuzziness : 99999);
  samplerHelper.initialise(std::shared_ptr<SampleGenerator>(sampler), SampleGenerator::unitHemisphereMap);
}

void ReflectionShader::setMirrorCol(const Vector3D& mirrorCol) {
  this->mirrorCol = mirrorCol;
}

void ReflectionShader::setFuzziness(float fuzziness) {
  this->fuzziness = fuzziness;
  initSampler(10);
}


Vector3D ReflectionShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  Vector3D phong = PhongShader::shadeSurface(hitRay, surfaceInfo, sceneDef);
  Vector3D reflectedLight = calcReflectedLight(hitRay, surfaceInfo, sceneDef);
  return phong + mirrorCol * reflectedLight;
}

using namespace mathernogl;

Vector3D ReflectionShader::calcReflectedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  //  construct orthonormal basis
  Vector3D w = reflect(hitRay->direction, surfaceInfo->normal);
  Vector3D u = crossProduct(Vector3D(0.0023, 1, 0.0048), w);
  u.makeUniform();
  Vector3D v = crossProduct(u, w);

  //  get next sample
  Vector3D sample (0, 0, 1);
  if (fuzziness > 0)
    sample = samplerHelper.getNextSample();

  //  use orthonormal basis to transform the sample hemisphere into the direction of the perfect reflection
  Vector3D sampleReflection = (u * sample.x) + (v * sample.y) + (w * sample.z);
  if (dotProduct(sampleReflection, surfaceInfo->normal) < 0.0)
    sampleReflection = (u * sample.x * -1.0f) + (v * sample.y * -1.0f) + (w * sample.z);

  //  trace a reflected ray to determine the colour of the light reaching the surface from that direction
  Ray reflectedRay = Ray::create(surfaceInfo->position, sampleReflection);
  Vector3D resultingLight(0);
  reflectedRay.depth = hitRay->depth + 1;
  Vector3D reflectedCol(0);
  if(RayTracer::traceRay(&reflectedRay, sceneDef, &reflectedCol))
    resultingLight = reflectedCol;
  else
    resultingLight = sceneDef->bgColour;
  return resultingLight;
}

