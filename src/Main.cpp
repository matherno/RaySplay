#include <mathernogl/MathernoGL.h>
#include <cameras/CameraOrthographic.h>
#include <cameras/CameraPerspective.h>
#include "Scene.h"
#include "ImageOutputFile.h"
#include "ImageOutputWindow.h"

int main() {
  mathernogl::RandomGenerator::setSeed();

#define WINDOW_OUTPUT
#ifdef WINDOW_OUTPUT
  ImageOutputWindow imageOutputFile;
#else
  ImageOutputFile imageOutputFile("Output.png");
#endif

  Scene scene;
  ViewDef* viewDef = scene.getViewDef();

#ifdef RS_DEBUG
  viewDef->width = 1024;
  viewDef->height = 512;
  viewDef->antiAliasingDegree = 1;
  viewDef->debugMode = true;
#else
  viewDef->width = 1920;
  viewDef->height = 1080;
  viewDef->antiAliasingDegree = 3;
  viewDef->debugMode = false;
#endif

  viewDef->fov = 1.13;
  viewDef->invGamma = 1.0f / 2.0f;

  SceneDef* sceneDef = scene.getSceneDef();
  sceneDef->bgColour.set(0, 0, 0);
  sceneDef->maxRayDepth = 4;

#ifdef SCENE_A
  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(0, 0.5, 0), Vector3D(0, -0.08, -1), Vector3D(0, 1, 0));
#endif

#ifdef SCENE_B
  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(-13.8, 5.5, 45), Vector3D(0.36, -0.1, -1), Vector3D(0, 1, 0));
#endif

#ifdef SCENE_C
  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(10,10, 50), Vector3D(0, -0.1, -1), Vector3D(0, 1, 0));
  camera->lookAt(Vector3D(0, 0, -30));
#endif

#ifdef SCENE_D
  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(0, 5, 0), Vector3D(0, 0, -1), Vector3D(0, 1, 0));
  camera->lookAt({0, 0, -15.5});
#endif

#ifdef SCENE_E
  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(0, 13, 10), Vector3D(0, -0.4, -1), Vector3D(0, 1, 0));
#endif

#ifdef SCENE_D
  light.reset(new DirectionLight(Vector3D(0.4), Vector3D(-1, -0.8, -0.4).getUniform()));
#endif
  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(13, 6, 25), Vector3D(0, 0, -1), Vector3D(0, 1, 0));
  camera->lookAt(Vector3D(-6, 5, 0));
  camera->setViewPlaneOffset(1.4);
  scene.setCamera(camera);

  scene.build();
  scene.render(&imageOutputFile);


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