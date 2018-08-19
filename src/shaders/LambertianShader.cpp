//
// Created by matherno on 10/08/17.
//

#include "LambertianShader.h"

LambertianShader::LambertianShader()
  {
  }

LambertianShader::LambertianShader(float r, float g, float b)
  {
  diffuseColour.set(r, g, b);
  }

LambertianShader::LambertianShader(const Vector3D& diffuseColour) : diffuseColour(diffuseColour)
  {
  }

LambertianShader::LambertianShader(TexturePtr diffuseTexture) : diffuseTexture(diffuseTexture)
  {
  }

Vector3D LambertianShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  Vector3D finalColour(0);
  Vector3D diffuse = getDiffuseColour(surfaceInfo);
  for(LightSourcePtr light : sceneDef->lights)
    {
    if(light->startShadingAtPoint(surfaceInfo, sceneDef) && !light->isPointInShadow(surfaceInfo, sceneDef))
      {
      double lightFactor = 1;
      if(!light->isAmbient())
        {
        const Vector3D lightDir = light->lightDirectionAtPoint(surfaceInfo, sceneDef);
        lightFactor = diffuseScatterer.getSurfaceRadianceFactor(surfaceInfo->normal, lightDir);
        }
      if(lightFactor > 0)
        finalColour += diffuse * (light->lightIntensityAtPoint(surfaceInfo, sceneDef) * lightFactor);
      }
    }
  return finalColour;
  }

Vector3D LambertianShader::getDiffuseColour() const
  {
  return diffuseColour;
  }

void LambertianShader::setDiffuseColour(const Vector3D& diffuseColour)
  {
  this->diffuseColour = diffuseColour;
  }

void LambertianShader::setDiffuseTexture(TexturePtr diffuseTexture)
  {
  this->diffuseTexture = diffuseTexture;
  }

Vector3D LambertianShader::getDiffuseColour(const SurfaceInfo* surfaceInfo) const
  {
  if (diffuseTexture)
    return diffuseTexture->getColour(surfaceInfo->texCoord);
  else
    return diffuseColour;
  }
