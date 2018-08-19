//
// Created by matherno on 21/08/17.
//

#include <RayTracer.h>
#include <Samplers.h>
#include "ReflectionShader.h"

ReflectionShader::ReflectionShader()
    : PhongShader()
  {
  setupScatterer();
  }

ReflectionShader::ReflectionShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, Vector3D mirrorCol)
    : PhongShader(diffuseColour, specularColour, specularExp, true), mirrorCol(mirrorCol)
  {
  setupScatterer();
  }

ReflectionShader::ReflectionShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp, Vector3D mirrorCol)
    : PhongShader(diffuseTexture, specularColour, specularExp, true), mirrorCol(mirrorCol)
  {
  setupScatterer();
  }

void ReflectionShader::setMirrorCol(const Vector3D& mirrorCol)
  {
  this->mirrorCol = mirrorCol;
  }

void ReflectionShader::setFuzziness(float fuzziness)
  {
  this->fuzziness = fuzziness;
  setupScatterer();
  }


Vector3D ReflectionShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  Vector3D phong = PhongShader::shadeSurface(hitRay, surfaceInfo, sceneDef);
  Vector3D reflectedLight = calcReflectedLight(hitRay, surfaceInfo, sceneDef);
  return phong + mirrorCol * reflectedLight;
  }

using namespace mathernogl;

Vector3D ReflectionShader::calcReflectedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  //  trace a reflected ray to determine the colour of the light reaching the surface from that direction
  const Vector3D sampleReflection = reflectiveScatterer->sampleIncomingLightDir(surfaceInfo->normal, hitRay->direction * -1);
  Ray reflectedRay = Ray::create(surfaceInfo->position, sampleReflection);
  reflectedRay.depth = hitRay->depth + 1;
  Vector3D reflectedCol(0);
  if(RayTracer::traceRay(&reflectedRay, sceneDef, &reflectedCol))
    return reflectedCol;
  return sceneDef->bgColour;
  }

void ReflectionShader::setupScatterer()
  {
  if (fuzziness > 0)
    reflectiveScatterer.reset(new LightScatterSpecular(1.0f / fuzziness));
  else
    reflectiveScatterer.reset(new LightScatterPerfectSpecular());
  }

