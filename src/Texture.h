#pragma once
//
// Created by matherno on 29/10/17.
//


#include "RaySplayConst.h"

class Texture {
public:
  virtual ~Texture() {}
  virtual Vector3D getColour(const Vector2D& texCoord) const = 0;
};

typedef std::shared_ptr<Texture> TexturePtr;
