//
// Created by matherno on 12/08/17.
//

#include <RayTracer.h>
#include "PointLight.h"

PointLight::PointLight() {
}

PointLight::PointLight(const Vector3D& colour, const Vector3D& position, float radius, float falloffExp /*= 1*/) : position(position), colour(colour), radius(radius), falloffExp(falloffExp) {
}

bool PointLight::isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  if(!castShadows)
    return false;
  Ray shadowRay = Ray::create(surfaceInfo->position, lightDirectionAtPoint(surfaceInfo, sceneDef));
  float hitTValue;
  if(RayTracer::traceShadowRay(&shadowRay, sceneDef, &hitTValue)){
    float lightTValue = (position.x - shadowRay.origin.x) / shadowRay.direction.x;
    if(hitTValue < lightTValue){
      return true;
    }
  }
  return false;
}

Vector3D PointLight::lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  return (position - surfaceInfo->position).getUniform();
}

Vector3D PointLight::lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  double lightDistance = (position - surfaceInfo->position).magnitude();
  double lightAttenuation = 1.0f - (lightDistance / radius);
  if(lightAttenuation < 0 || lightAttenuation > 1)
    return Vector3D();
  lightAttenuation = pow(lightAttenuation, falloffExp);
  return colour * lightAttenuation;
}

Vector3D PointLight::getPosition() const {
  return position;
}

void PointLight::setPosition(const Vector3D& position) {
  this->position = position;
}

Vector3D PointLight::getColour() const {
  return colour;
}

void PointLight::setColour(const Vector3D& colour) {
  this->colour = colour;
}

