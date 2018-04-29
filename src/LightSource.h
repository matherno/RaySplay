//
// Created by matherno on 9/08/17.
//

#pragma once

#include <memory>

class LightSource;
typedef std::shared_ptr<LightSource> LightSourcePtr;

#include "SurfaceInfo.h"
#include "SceneDef.h"
#include "RaySplayConst.h"

class LightSource{
public:
  bool castShadows = true;

  LightSource(){}
  virtual ~LightSource(){}

  virtual //  returns true if this light source is ambient / doesn't have a source direction
  bool isAmbient() { return false; }

  //  returns true if the given surface point is in this lights shadow
  virtual bool isPointInShadow(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) = 0;

  //  returns the unit direction from the given point that the light is coming from
  //  shouldn't be used if this light source is found to be ambient
  virtual Vector3D lightDirectionAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) = 0;

  //  returns the intensity(colour) of the light at the given point
  virtual Vector3D lightIntensityAtPoint(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) = 0;
};

