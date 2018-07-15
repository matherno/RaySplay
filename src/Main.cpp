#include <mathernogl/MathernoGL.h>
#include <cameras/CameraOrthographic.h>
#include <cameras/CameraPerspective.h>
#include "Scene.h"
#include "ImageOutputFile.h"
#include "ImageOutputWindow.h"
#include "SceneLoader.h"

int main() {
  mathernogl::RandomGenerator::setSeed();
  mathernogl::resetLog();

#define WINDOW_OUTPUT
#ifdef WINDOW_OUTPUT
  ImageOutputWindow imageOutputFile;
#else
  ImageOutputFile imageOutputFile("Output.png");
#endif

  Scene scene;
  ViewDef* viewDef = scene.getViewDef();

  viewDef->width = 1024;
  viewDef->height = 512;
  viewDef->antiAliasingDegree = 1;
#ifdef RS_DEBUG
  viewDef->debugMode = true;
#else
  viewDef->debugMode = false;
#endif
  viewDef->fov = 1.13;
  viewDef->invGamma = 1.0f / 2.0f;

  SceneDef* sceneDef = scene.getSceneDef();
  sceneDef->bgColour.set(0, 0, 0);
  sceneDef->maxRayDepth = 4;

  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(13, 6, 25), Vector3D(0, 0, -1), Vector3D(0, 1, 0));
  camera->lookAt(Vector3D(-6, 4, 0));
  camera->setViewPlaneOffset(1.4);
  scene.setCamera(camera);

  if (scene.build("WorkshopScene.xml"))
    scene.render(&imageOutputFile);
  else
    return 0;

#ifndef WINDOW_OUTPUT
  std::time_t rawtime;
  std::tm* timeinfo;
  char buffer [15];
  std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);
  std::strftime(buffer,15,"%y%m%d%H%M%S",timeinfo);
  std::string timeFormatted = buffer;
  imageOutputFile.saveCurrentStateToFile("image_stash/output/Output_" + timeFormatted + ".png");
#endif

  return 0;
}