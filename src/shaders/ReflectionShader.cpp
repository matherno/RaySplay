//
// Created by matherno on 21/08/17.
//

#include <RayTracer.h>
#include "ReflectionShader.h"

ReflectionShader::ReflectionShader() : PhongShader() {

}

ReflectionShader::ReflectionShader(const Vector3D& diffuseColour, const Vector3D& specularColour, float specularExp, bool blinnPhong)
    : PhongShader(diffuseColour, specularColour, specularExp, blinnPhong) {

}

//todo - do this properly
Vector3D ReflectionShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  Vector3D phongColour = PhongShader::shadeSurface(hitRay, surfaceInfo, sceneDef);
  Vector3D reflectionDir = mathernogl::reflect(hitRay->direction, surfaceInfo->normal);
  Vector3D reflectionColour(0);
  Ray reflectionRay = Ray::create(surfaceInfo->position, reflectionDir);
  reflectionRay.depth = hitRay->depth + 1;
  if(RayTracer::traceRay(&reflectionRay, sceneDef, &reflectionColour)){
    return (phongColour + reflectionColour) / 2.0f;
  }
  else{
    return (phongColour + sceneDef->bgColour) / 2.0f;
  }
}
