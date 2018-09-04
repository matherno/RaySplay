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

  //  returns true if the given surface point is lit by this light, if surface is lit, then...
  //  lightDirection will be the unit direction from the given surface that the light is coming from, if not ambient
  //  lightIntensity wil be the intensity(colour) of the light at the given surface
  virtual bool lightAtSurface(const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef, Vector3D* lightIntensity, Vector3D* lightDirection) = 0;


};

