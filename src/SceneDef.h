#pragma once
//
// Created by matherno on 9/08/17.
//

struct SceneDef;

#include "Geometry.h"
#include "LightSource.h"
#include "RaySplayConst.h"

struct SceneDef {
  std::vector<GeometryPtr> geometries;
  std::vector<LightSourcePtr> lights;

  Vector3D bgColour;
  int maxRayDepth = 4;
};
