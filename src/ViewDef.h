#pragma once
//
// Created by matherno on 2/08/17.
//

#include <mathernogl/MathernoGL.h>

struct ViewDef {
  uint width = 0;
  uint height = 0;
  float fov = 1;
  float invGamma = 1;
  int antiAliasingDegree = 1;
  bool draftMode = false;

  float getAspectRatio() const { return (float) width / height; }
  float getPixelSize() const { return fov / width; }
};
