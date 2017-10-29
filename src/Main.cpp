#include <mathernogl/MathernoGL.h>
#include <cameras/CameraOrthographic.h>
#include <cameras/CameraPerspective.h>
#include "Scene.h"
#include "ImageOutputFile.h"

int main() {
  mathernogl::RandomGenerator::setSeed();

  ImageOutputFile imageOutputFile("Output.png");

  Scene scene;
  ViewDef* viewDef = scene.getViewDef();

#ifdef RS_DEBUG
  viewDef->width = 1366;
  viewDef->height = 768;
  viewDef->antiAliasingDegree = 1;
#else
  viewDef->width = 1920;
  viewDef->height = 1080;
  viewDef->antiAliasingDegree = 3;
#endif

  viewDef->fov = 1.13;
  viewDef->invGamma = 1.0f / 2.2f;

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
//  std::shared_ptr<CameraPerspective> camera = std::make_shared<CameraPerspective>(Vector3D(30, 0, 0), Vector3D(-1, 0, 0), Vector3D(0, 1, 0));
//  camera->lookAt(Vector3D(0, 0, 0));
  camera->setViewPlaneOffset(2);
  scene.setCamera(camera);

  scene.build();
  scene.render(&imageOutputFile);

  std::time_t rawtime;
  std::tm* timeinfo;
  char buffer [15];
  std::time(&rawtime);
  timeinfo = std::localtime(&rawtime);
  std::strftime(buffer,15,"%y%m%d%H%M%S",timeinfo);
  std::string timeFormatted = buffer;

  imageOutputFile.saveCurrentStateToFile("image_stash/Output_" + timeFormatted + ".png");

  return 0;
}