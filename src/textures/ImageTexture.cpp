//
// Created by matherno on 29/10/17.
//

#include "ImageTexture.h"

ImageTexture::ImageTexture(ColourMapPtr colourMap, uint width, uint height) : width(width), height(height) {
  this->colourMap = std::move(colourMap);
}

Vector3D ImageTexture::getColour(const Vector2D& texCoord) const {
  int column = (int) std::floor(texCoord.x * width) % width;
  int row = (int) std::floor(texCoord.y * height) % height;
  return (*colourMap)[column + row * width];
}
