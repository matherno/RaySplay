//
// Created by matherno on 10/08/17.
//

#include "AmbientLight.h"

AmbientLight::AmbientLight(float r, float g, float b) {
  colour.set(r, g, b);
}

AmbientLight::AmbientLight(const Vector3D& colour) : colour(colour) {

}

bool AmbientLight::isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  return false;
}

Vector3D AmbientLight::lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  ASSERT(false, "Requesting the direction of ambient light. Need to check isAmbient().");
  return Vector3D();
}

Vector3D AmbientLight::lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  return colour;
}

const Vector3D AmbientLight::getColour() const {
  return colour;
}

void AmbientLight::setColour(const Vector3D& colour) {
  this->colour = colour;
}

