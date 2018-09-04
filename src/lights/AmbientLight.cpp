//
// Created by matherno on 10/08/17.
//

#include "AmbientLight.h"

AmbientLight::AmbientLight(float r, float g, float b) {
  colour.set(r, g, b);
}

AmbientLight::AmbientLight(const Vector3D& colour) : colour(colour) {

}

const Vector3D AmbientLight::getColour() const {
  return colour;
}

void AmbientLight::setColour(const Vector3D& colour) {
  this->colour = colour;
}

bool AmbientLight::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  if (lightIntensity)
    *lightIntensity = colour;
  return true;
  }

