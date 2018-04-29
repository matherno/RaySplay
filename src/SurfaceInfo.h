//
// Created by matherno on 31/07/17.
//

#pragma once

struct SurfaceInfo;

#include <mathernogl/MathernoGL.h>
#include "Shader.h"
#include "RaySplayConst.h"

struct SurfaceInfo{
  Vector3D position;
  Vector3D normal;
  Vector2D texCoord;
  ShaderPtr material;
};
