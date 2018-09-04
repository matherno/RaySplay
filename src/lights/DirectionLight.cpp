//
// Created by matherno on 10/08/17.
//

#include <RayTracer.h>
#include "DirectionLight.h"

DirectionLight::DirectionLight(const Vector3D& colour, const Vector3D& direction)
    : colour(colour) {
  setDirection(direction);
}

DirectionLight::DirectionLight() : colour(0,0,0) {
  setDirection({0,1,0});
}

const Vector3D DirectionLight::getColour() const {
  return colour;
}

void DirectionLight::setColour(const Vector3D& colour) {
  this->colour = colour;
}

const Vector3D DirectionLight::getDirection() const {
  return direction;
}

void DirectionLight::setDirection(const Vector3D& direction) {
  this->direction = direction;
  this->direction.makeUniform();
  directionFlipped = this->direction * -1;
}

bool DirectionLight::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  Ray shadowRay = Ray::create(surfaceInfo->position, directionFlipped);
  double tValue;
  if(castShadows && RayTracer::traceShadowRay(&shadowRay, sceneDef, &tValue))
    return false;

  if (lightIntensity)
    *lightIntensity = colour;
  if (lightDirection)
    *lightDirection = directionFlipped;
  return true;
  }