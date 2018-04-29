#pragma once
//
// Created by matherno on 1/08/17.
//

#include "ImageOutput.h"
#include "mathernogl/MathernoGL.h"

class ImageOutputFile : public ImageOutput {
private:
  std::string outputFilePath;
  mathernogl::Image::ColourVector pixels;
  std::unique_ptr<mathernogl::Image> image;

public:
  ImageOutputFile(const std::string& outputFilePath);

  bool prepare(uint imageWidth, uint imageHeight) override;
  void paintPixel(uint x, uint y, const Vector3D& colour) override;
  bool finalise() override;

  bool saveCurrentStateToFile(const std::string& filePath);
};
