#pragma once

#ifndef NDEBUG
#define RS_DEBUG
#endif

//#define SCENE_A
#define SCENE_B
//#define SCENE_C
//#define SCENE_D
//#define SCENE_E

//
// Created by matherno on 1/08/17.
//

#include <geometries/PrimitiveSphere.h>
#include <geometries/PrimitivePlane.h>
#include "ImageOutput.h"
#include "ViewDef.h"
#include "SampleGenerator.h"
#include "Camera.h"
#include "AccelerationStructure.h"

class Scene {
private:
  ViewDef viewDef;
  SceneDef sceneDef;
  std::shared_ptr<Camera> camera;

public:
  Scene();

  void build();
  void render(ImageOutput* imageOutput);
  void paintPixel(uint x, uint y, const Vector3D& colour, ImageOutput* imageOutput);

  ViewDef* getViewDef(){ return &viewDef; }
  SceneDef* getSceneDef(){ return &sceneDef; }
  void setCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }
  std::shared_ptr<Camera>& getCamera() { return camera; }
};
