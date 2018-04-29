#pragma once
//
// Created by matherno on 1/08/17.
//

#include <mathernogl/MathernoGL.h>
#include "RaySplayConst.h"

class ImageOutput {
public:

  //  prepares the image output to start receiving paintPixel calls
  virtual bool prepare(uint imageWidth, uint imageHeight) = 0;

  //  paints the given colour to the given pixel
  virtual void paintPixel(uint x, uint y, const Vector3D& colour) = 0;

  //  called once all calls to paintPixel is done to finalise output
  virtual bool finalise() = 0;

  //  is called frequently and will bail the process if returned true (without a call to finalise())
  virtual bool queryBail() { return false; }
};
