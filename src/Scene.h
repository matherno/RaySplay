#pragma once

#ifndef NDEBUG
#define RS_DEBUG
#endif

//#define SCENE_A     //  original
//#define SCENE_B     //  sphere and boxes
//#define SCENE_C       //  lots of spheres
//#define SCENE_D     //  random trees
#define SCENE_E     //  single model (plant pot)

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
#include "ResourceManager.h"

class Scene {
private:
  ViewDef viewDef;
  SceneDef sceneDef;
  std::shared_ptr<Camera> camera;
  ResourceManager resourceManager;

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
