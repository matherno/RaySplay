#pragma once
//
// Created by matherno on 29/10/17.
//


#include <Texture.h>

typedef std::unique_ptr<mathernogl::Image::ColourVector> ColourMapPtr;

class ImageTexture : public Texture {
protected:
  ColourMapPtr colourMap;
  uint width;
  uint height;

public:
  ImageTexture(ColourMapPtr colourMap, uint width, uint height);

  inline Vector3D getTexelColour(uint col, uint row) const;
  virtual Vector3D getColour(const Vector2D& texCoord) const override;

  static TexturePtr create(string filePath);
};
