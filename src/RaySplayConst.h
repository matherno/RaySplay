//
// Created by matherno on 31/07/17.
//

#pragma once

#include <mathernogl/MathernoGL.h>

#define EPSILON 1e-2

#define RAND_INT(min,max)     mathernogl::RandomGenerator::randomInt(min,max)
#define RAND_FLOAT(min,max)   mathernogl::RandomGenerator::randomFloat(min,max)

typedef mathernogl::Vector3D Vector3D;
typedef mathernogl::Vector2D Vector2D;