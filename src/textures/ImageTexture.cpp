//
// Created by matherno on 29/10/17.
//

#include "ImageTexture.h"

ImageTexture::ImageTexture(ColourMapPtr colourMap, uint width, uint height) : Texture(), width(width), height(height) {
  this->colourMap = std::move(colourMap);
}

Vector3D ImageTexture::getTexelColour(uint col, uint row) const {
  return (*colourMap)[(col % width) + (row % height) * width];
}

Vector3D ImageTexture::getColour(const Vector2D& texCoord) const {
  using namespace mathernogl;
  uint column = (uint) std::floor(texCoord.x * width);
  uint row = (uint) std::floor((1.0 - texCoord.y) * height);

  if (textureFiltering == NEAREST) {
    return getTexelColour(column, row);
  }
  else if (textureFiltering == LINEAR){
    float stub;
    float texelPosX = std::modf(texCoord.x * width, &stub) - 0.5f;
    float texelPosY = std::modf(texCoord.y * height, &stub) - 0.5f;
    int sampleXOffset = texelPosX < 0 ? -1 : 1;
    int sampleYOffset = texelPosY < 0 ? -1 : 1;
    float totalFactor = 0;
    Vector3D outCol(0);

    auto addFilteredColour = [this, &outCol, &totalFactor, texelPosX, texelPosY, column, row] (uint colOffset, uint rowOffset) {
      float factor = (float)(1.0 / pow(texelPosX - colOffset, 2) + pow(texelPosY - rowOffset, 2));
      outCol += getTexelColour(column+colOffset, row+rowOffset) * factor;
      totalFactor += factor;
    };

    addFilteredColour(0, 0);
    addFilteredColour(sampleXOffset, 0);
    addFilteredColour(0, sampleYOffset);
    addFilteredColour(sampleXOffset, sampleYOffset);
    return outCol / totalFactor;
  }
  else {
    return ERROR_COLOUR;
  }
}
