#pragma once
//
// Created by matherno on 9/08/17.
//

#include <memory>

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

#include <mathernogl/MathernoGL.h>
#include "SurfaceInfo.h"
#include "SceneDef.h"
#include "RaySplayConst.h"

static uint nextShaderID = 1;

class Shader {
public:
  const uint id;
  Shader() : id (nextShaderID++) {}
  virtual ~Shader(){}

  //  shades the given surface, as seen through the given ray, and returns the resulting colour
  virtual Vector3D shadeSurface(const Ray* hitRay, const SurfaceInfo* surfaceInfo, const SceneDef* sceneDef) = 0;

  //  used for simplified rendering of a surface
  virtual Vector3D getSimplifiedSurfaceColour() const = 0;

  virtual bool isEmissive() const { return false; }
};