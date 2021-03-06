//
// Created by matherno on 12/08/17.
//

#include <RayTracer.h>
#include "PointLight.h"

PointLight::PointLight() {
}

PointLight::PointLight(const Vector3D& colour, const Vector3D& position, float radius, float falloffExp /*= 1*/) : position(position), colour(colour), radius(radius), falloffExp(falloffExp) {
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

bool PointLight::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  return lightAtSurface(surfaceInfo, sceneDef, position, lightIntensity, lightDirection);
  }

bool PointLight::lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, const Vector3D& lightPos, Vector3D* lightIntensity, Vector3D* lightDirection)
  {
  const Vector3D dir = (lightPos - surfaceInfo->position).getUniform();

  Ray shadowRay = Ray::create(surfaceInfo->position, dir);
  double hitTValue;
  if(castShadows && RayTracer::traceShadowRay(&shadowRay, sceneDef, &hitTValue))
    {
    double lightTValue = (lightPos.x - shadowRay.origin.x) / shadowRay.direction.x;
    if(hitTValue < lightTValue)
      return false;
    }

  if (lightDirection)
    *lightDirection = dir;

  if (lightIntensity)
    {
    const double lightDistance = (lightPos - surfaceInfo->position).magnitude();
    double lightAttenuation = 1.0f - (lightDistance / radius);
    if (lightAttenuation < 0 || lightAttenuation > 1)
      return false;
    lightAttenuation = pow(lightAttenuation, falloffExp);
    *lightIntensity = colour * lightAttenuation;
    }

  return true;
  }

string PointLight::constructGLSLLightSurface() const
  {
  return constructGLSLLightSurface(position);
  }

string PointLight::constructGLSLLightSurface(const Vector3D& lightPos) const
  {
  return ""
    "vec3 lightPos = " + glslVec3(lightPos) + ";"
    "vec3 hitColour, hitNormal;"
    "vec3 surfaceToLightDir = normalize(lightPos - surfacePos);"
    "float tValue = hitTestScene(surfacePos, surfaceToLightDir, hitNormal, hitColour);"
    "float lightTValue = (lightPos.x - surfacePos.x) / surfaceToLightDir.x;\n"
    "if (tValue < 0 || tValue > lightTValue)"
    "  {"
    "  "
    "  float lightDistance = length(lightPos - surfacePos);\n"
    "  float lightAttenuation = 1.0 - (lightDistance / " + std::to_string(radius) + ");\n"
    "  if (lightAttenuation >= 0 && lightAttenuation <= 1)"
    "    {"
    "    lightAttenuation = pow(lightAttenuation, " + std::to_string(falloffExp) + ");\n"
    "    lightCol = " + glslVec3(colour) + " * lightAttenuation;"
    "    lightDir = surfaceToLightDir;"
    "    }"
    "  }";
  }

