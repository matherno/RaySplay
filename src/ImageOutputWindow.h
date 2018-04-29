#pragma once

#include "ImageOutput.h"
#include "ImageOutputFile.h"

/*
*   
*/

class ImageOutputWindow : public ImageOutput
  {
private:
  std::unique_ptr<mathernogl::Window> window;
  std::shared_ptr<mathernogl::Texture> texture;
  std::unique_ptr<mathernogl::ScreenTextureRenderer> renderer;
  std::unique_ptr<ImageOutputFile> fileOutput;

public:
  virtual bool prepare(uint imageWidth, uint imageHeight) override;
  virtual void paintPixel(uint x, uint y, const Vector3D& colour) override;
  virtual bool finalise() override;
  virtual bool queryBail() override;

protected:
  void updateWindow();
  };
