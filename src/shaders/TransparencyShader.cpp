//
// Created by matt on 5/09/18.
//

#include <RayTracer.h>
#include "TransparencyShader.h"

TransparencyShader::TransparencyShader() : ReflectionShader()
  {
  setupScatterer();
  }

TransparencyShader::TransparencyShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp)
  : ReflectionShader(diffuseColour, specularColour, specularExp, true)
  {
  setupScatterer();
  }

TransparencyShader::TransparencyShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp)
  : ReflectionShader(diffuseTexture, specularColour, specularExp, true)
  {
  setupScatterer();
  }

void TransparencyShader::setInternalCol(const Vector3D& internalCol)
  {
  internalColour = internalCol;
  }

void TransparencyShader::setRefractionIndex(double idx)
  {
  refractionIndex = idx;
  setupScatterer();
  }

Vector3D TransparencyShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  const Vector3D reflectionLight = ReflectionShader::shadeSurface(hitRay, surfaceInfo, sceneDef);
  const Vector3D refractedLight = calcRefractedLight(hitRay, surfaceInfo, sceneDef);
  return reflectionLight + internalColour * refractedLight;
  }

static bool equalsZero(double value)
  {
  return fabs(value) <= 1e-10;
  }

Vector3D TransparencyShader::calcRefractedLight(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  //  trace a refracted ray to determine the colour of the light reaching the surface from that direction
  using namespace mathernogl;
  const Vector3D sampleRefraction = refractiveScatterer->sampleIncomingLightDir(surfaceInfo->normal, hitRay->direction * -1);
  if (equalsZero(sampleRefraction.x) && equalsZero(sampleRefraction.y) && equalsZero(sampleRefraction.z))
    return Vector3D(0);   // must be total internal reflection (so no refraction ray)

  Ray refractionRay = Ray::create(surfaceInfo->position, sampleRefraction);
  refractionRay.depth = hitRay->depth + 1;
  Vector3D reflectedCol(0);
  bool hitBackground = false;
  const double radianceFactor = refractiveScatterer->getSurfaceRadianceFactor(surfaceInfo->normal, hitRay->direction*-1, sampleRefraction);
  if(RayTracer::traceRay(&refractionRay, sceneDef, &reflectedCol, &hitBackground))
    return reflectedCol * radianceFactor;
  if (hitBackground)
    return sceneDef->bgColour * radianceFactor;
  else
    return Vector3D(0);
  }
void TransparencyShader::setupScatterer()
  {
  refractiveScatterer.reset(new LightScatterRefraction(refractionIndex));
  }

Vector3D TransparencyShader::getSimplifiedSurfaceColour() const
  {
  return internalColour;
  }

