#pragma once
//
// Created by matherno on 29/10/17.
//


#include "RaySplayConst.h"

using mathernogl::TextureFiltering;
using mathernogl::TextureWrapping;

class Texture {
protected:
  TextureFiltering textureFiltering = TextureFiltering::NEAREST;
  TextureWrapping textureWrapping = TextureWrapping::REPEAT;

public:
  virtual ~Texture() {}
  virtual Vector3D getColour(const Vector2D& texCoord) const = 0;

  TextureFiltering getTextureFiltering() const { return textureFiltering; }
  TextureWrapping getTextureWrapping() const { return textureWrapping; }
  void setTextureFiltering(TextureFiltering textureFiltering) { this->textureFiltering = textureFiltering; }
  void setTextureWrapping(TextureWrapping textureWrapping) { this->textureWrapping = textureWrapping; }
};

typedef std::shared_ptr<Texture> TexturePtr;
