//
// Created by matherno on 31/07/17.
//

#pragma once

#include <mathernogl/MathernoGL.h>

#define EPSILON 1e-5

#define RAND_INT(min,max)     mathernogl::RandomGenerator::randomInt(min,max)
#define RAND_FLOAT(min,max)   mathernogl::RandomGenerator::randomFloat(min,max)

typedef mathernogl::Vector3D Vector3D;
typedef mathernogl::Vector2D Vector2D;
typedef std::string string;

#define ERROR_COLOUR Vector3D(0.9, 0.0, 0.6)

static string glslVec3(const Vector3D& value)
  {
  return mathernogl::stringFormat("vec3(%.5f, %.5f, %.5f)", value.x, value.y, value.z);
  }