//
// Created by matherno on 12/08/17.
#include "PhongShader.h"

PhongShader::PhongShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, bool blinnPhong /*= true*/)
    : diffuseColour(diffuseColour), specularColour(specularColour), specularExp(specularExp), blinnPhong(blinnPhong)
  {
  setupScatterers();
  }

PhongShader::PhongShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp, bool blinnPhong /*= true*/)
    : diffuseTexture(diffuseTexture), specularColour(specularColour), specularExp(specularExp), blinnPhong(blinnPhong)
  {
  setupScatterers();
  }

PhongShader::PhongShader(double diffuseR, double diffuseG, double diffuseB)
  : PhongShader(Vector3D(diffuseR, diffuseG, diffuseB))
  {

  }

PhongShader::PhongShader(const Vector3D& diffuseColour)
  : PhongShader(diffuseColour, Vector3D(0, 0, 0), 0)
  {

  }

PhongShader::PhongShader()
  {
  setupScatterers();
  }

Vector3D PhongShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef)
  {
  Vector3D finalColour(0);
  const Vector3D viewDir = hitRay->direction * -1;
  const Vector3D diffuseCol = getDiffuseColour(surfaceInfo);
  for(LightSourcePtr light : sceneDef->lights)
    {
    Vector3D lightDir, lightCol;
    if(light->lightAtSurface(surfaceInfo, sceneDef, &lightCol, &lightDir))
      {
      if(light->isAmbient())
        {
        finalColour += diffuseCol * lightCol;
        }
      else
        {
        const Vector3D diffuseLight = lightCol * diffuseScatterer->getSurfaceRadianceFactor(surfaceInfo->normal, viewDir, lightDir);
        const Vector3D specularLight = lightCol * specularScatterer->getSurfaceRadianceFactor(surfaceInfo->normal, viewDir, lightDir);
        finalColour += (diffuseCol * diffuseLight) + (specularColour * specularLight);
        }
      }
    }
  return finalColour;
  }

Vector3D PhongShader::getDiffuseColour() const
  {
  return diffuseColour;
  }

void PhongShader::setDiffuseColour(const Vector3D& diffuseColour)
  {
  this->diffuseColour = diffuseColour;
  }

Vector3D PhongShader::getSpecularColour() const
  {
  return specularColour;
  }

void PhongShader::setSpecularColour(const Vector3D& specularColour)
  {
  this->specularColour = specularColour;
  }

float PhongShader::getSpecularExp() const
  {
  return specularExp;
  }

void PhongShader::setSpecularExp(float specularExp)
  {
  this->specularExp = specularExp;
  setupScatterers();
  }

void PhongShader::setDiffuseTexture(TexturePtr diffuseTexture)
  {
  this->diffuseTexture = diffuseTexture;
  }

Vector3D PhongShader::getDiffuseColour(const SurfaceInfo* surfaceInfo) const
  {
  if (diffuseTexture)
    return diffuseTexture->getColour(surfaceInfo->texCoord);
  else
    return diffuseColour;
  }

void PhongShader::setupScatterers()
  {
  diffuseScatterer.reset(new LightScatterDiffuse());
  specularScatterer.reset(new LightScatterSpecular(specularExp));
  }

