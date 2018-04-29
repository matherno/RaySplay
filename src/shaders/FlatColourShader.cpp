//
// Created by matherno on 9/08/17.
//

#include "FlatColourShader.h"

FlatColourShader::FlatColourShader(float r, float g, float b) {
  colour.set(r, g, b);
}

const mathernogl::Vector3D& FlatColourShader::getColour() const {
  return colour;
}

void FlatColourShader::setColour(const mathernogl::Vector3D& colour) {
  this->colour = colour;
}

void FlatColourShader::setColour(float r, float g, float b) {
  colour.set(r, g, b);
}

mathernogl::Vector3D FlatColourShader::shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) {
  return colour;
}
