#pragma once
//
// Created by matherno on 3/08/17.
//


#include "SceneDef.h"

class RayTracer {
public:
  //  traces the given scene definition, to find a result colour at the closest hit along the given ray
  static bool traceRay(const Ray* ray, const SceneDef* sceneDef, Vector3D* resultColour, bool* hitBackground = nullptr);

  //  traces along the given ray and returns true if it hits an object in the given scene definition, giving that hits 't' value
  //  ignores the rays depth
  //  more efficient than traceRay() as it doesn't need to do any shading
  static bool traceShadowRay(const Ray* ray, const SceneDef* sceneDef, double* tHitValue);
};