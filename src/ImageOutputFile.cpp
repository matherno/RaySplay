//
// Created by matherno on 1/08/17.
//

#include "ImageOutputFile.h"

#define POS_TO_INDEX(x,y) x + (y * image->getWidth())

ImageOutputFile::ImageOutputFile(const std::string& outputFilePath) : outputFilePath(outputFilePath) {

}

bool ImageOutputFile::prepare(uint imageWidth, uint imageHeight) {
  pixels.clear();
  pixels.reserve(imageWidth * imageHeight);
  for(int pixelNum = 0; pixelNum < imageWidth * imageHeight; ++pixelNum)
    pixels.emplace_back(0,0,0,1);
  image = mathernogl::Image::create(imageWidth, imageHeight, 24);
  return (bool)image;
}

void ImageOutputFile::paintPixel(uint x, uint y, const Vector3D& colour) {
  pixels[POS_TO_INDEX(x, y)] = mathernogl::Vector4D(colour, 1);
}

bool ImageOutputFile::finalise() {
  return saveCurrentStateToFile(outputFilePath);
}

bool ImageOutputFile::saveCurrentStateToFile(const std::string& filePath) {
  if(image->writeColourData(&pixels)){
    mathernogl::Image::save(image.get(), filePath);
    return true;
  }
  return false;
}

