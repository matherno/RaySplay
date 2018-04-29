//
// Created by matt on 28/04/18.
//

#include "ImageOutputWindow.h"

bool ImageOutputWindow::prepare(uint imageWidth, uint imageHeight)
  {
  window.reset(mathernogl::initGL("RaySplay", imageWidth, imageHeight, false, false));
  if (!window)
    return false;

  texture.reset(mathernogl::createEmptyTexture(imageWidth, imageHeight, mathernogl::NEAREST));
  if (!texture)
    return false;

  renderer = std::make_unique<mathernogl::ScreenTextureRenderer>(true);
  renderer->init();
  renderer->setTexture(texture);

  fileOutput = std::make_unique<ImageOutputFile>("Output.png");
  return fileOutput->prepare(imageWidth, imageHeight);
  }

void ImageOutputWindow::paintPixel(uint x, uint y, const Vector3D& colour)
  {
  const int redIndex = texture->bytesPerPixel * (x + (texture->height - y - 1) * texture->width);
  texture->bytes.get()[redIndex] = mathernogl::byte(colour.x * 255);
  texture->bytes.get()[redIndex+1] = mathernogl::byte(colour.y * 255);
  texture->bytes.get()[redIndex+2] = mathernogl::byte(colour.z * 255);

  if (x == 0 && y % 20 == 0)
    updateWindow();

  fileOutput->paintPixel(x, y, colour);
  }

bool ImageOutputWindow::finalise()
  {
  fileOutput->finalise();
  std::time_t rawtime;
  std::tm* timeinfo;
  char buffer [15];
  std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);
  std::strftime(buffer,15,"%y%m%d%H%M%S",timeinfo);
  std::string timeFormatted = buffer;
  fileOutput->saveCurrentStateToFile("image_stash/output/Output_" + timeFormatted + ".png");

  updateWindow();
  while(window->isOpen())
    window->update();
  return true;
  }

bool ImageOutputWindow::queryBail()
  {
  return !window->isOpen();
  }

void ImageOutputWindow::updateWindow()
  {
  mathernogl::updateTexture(texture.get());
  window->clear();
  renderer->render();
  window->update();
  }
