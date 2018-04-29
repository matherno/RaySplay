//
// Created by matherno on 12/08/17.
//

#include "PhongShader.h"

PhongShader::PhongShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, bool blinnPhong /*= true*/)
    : diffuseColour(diffuseColour), specularColour(specularColour), specularExp(specularExp), blinnPhong(blinnPhong) {
}

PhongShader::PhongShader(TexturePtr diffuseTexture, const Vector3D& specularColour, float specularExp, bool blinnPhong /*= true*/)
    : diffuseTexture(diffuseTexture), specularColour(specularColour), specularExp(specularExp), blinnPhong(blinnPhong) {
}

PhongShader::PhongShader() {
}

Vector3D PhongShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  Vector3D finalColour(0);
  Vector3D diffuse = getDiffuseColour(surfaceInfo);
  for(LightSourcePtr light : sceneDef->lights){
    if(!light->isPointInShadow(surfaceInfo, sceneDef)){
      Vector3D lightIntensity = light->lightIntensityAtPoint(surfaceInfo, sceneDef);
      if(light->isAmbient()){
        finalColour += diffuse * lightIntensity;
      }
      else{
        Vector3D lightDir = light->lightDirectionAtPoint(surfaceInfo, sceneDef);
        float lightDotNormal = mathernogl::dotProduct(surfaceInfo->normal, lightDir);
        if(lightDotNormal > 0){
          lightDotNormal *= lightDotNormal;
          float cosSpecularAngle;
          if(blinnPhong){
            Vector3D halfVector = lightDir + (hitRay->direction * -1);
            halfVector.makeUniform();
            cosSpecularAngle = mathernogl::dotProduct(halfVector, surfaceInfo->normal);
          }
          else{
            Vector3D reflectionDir = mathernogl::reflect(lightDir, surfaceInfo->normal);
            cosSpecularAngle = (float)fabs(mathernogl::dotProduct(reflectionDir, hitRay->direction * -1));
          }
          finalColour += (diffuse + specularColour * (float)pow(cosSpecularAngle, specularExp)) * (lightIntensity * lightDotNormal);
        }
      }
    }
  }
  return finalColour;
}

Vector3D PhongShader::getDiffuseColour() const {
  return diffuseColour;
}

void PhongShader::setDiffuseColour(const Vector3D& diffuseColour) {
  this->diffuseColour = diffuseColour;
}

Vector3D PhongShader::getSpecularColour() const {
  return specularColour;
}

void PhongShader::setSpecularColour(const Vector3D& specularColour) {
  this->specularColour = specularColour;
}

float PhongShader::getSpecularExp() const {
  return specularExp;
}

void PhongShader::setSpecularExp(float specularExp) {
  this->specularExp = specularExp;
}

void PhongShader::setDiffuseTexture(TexturePtr diffuseTexture) {
  this->diffuseTexture = diffuseTexture;
}

Vector3D PhongShader::getDiffuseColour(const SurfaceInfo* surfaceInfo) const {
  if (diffuseTexture)
    return diffuseTexture->getColour(surfaceInfo->texCoord);
  else
    return diffuseColour;
}

