//
// Created by matherno on 10/08/17.
//

#include "LambertianShader.h"

LambertianShader::LambertianShader() {
}

LambertianShader::LambertianShader(float r, float g, float b) {
  diffuseColour.set(r, g, b);
}

LambertianShader::LambertianShader(const Vector3D& diffuseColour) : diffuseColour(diffuseColour) {
}

Vector3D LambertianShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  Vector3D finalColour(0);
  for(LightSourcePtr light : sceneDef->lights){
    if(!light->isPointInShadow(surfaceInfo, sceneDef)){
      float lightFactor = 1;
      if(!light->isAmbient()){
        lightFactor = mathernogl::dotProduct(surfaceInfo->normal, light->lightDirectionAtPoint(surfaceInfo, sceneDef));
      }
      if(lightFactor > 0){
        finalColour += diffuseColour * (light->lightIntensityAtPoint(surfaceInfo, sceneDef) * lightFactor);
      }
    }
  }
  return finalColour;
}

Vector3D LambertianShader::getDiffuseColour() const {
  return diffuseColour;
}

void LambertianShader::setDiffuseColour(const Vector3D& diffuseColour) {
  this->diffuseColour = diffuseColour;
}