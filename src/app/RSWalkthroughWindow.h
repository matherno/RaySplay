#pragma once

#include <RaySplayConst.h>
#include <Scene.h>

/*
*   
*/

class RSWalkthroughWindow
  {
private:
  std::unique_ptr<mathernogl::Window> window;
  mathernogl::ShaderProgram shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::InputHandler inputHandler;

  Vector3D camPosition;
  double yaw = 0;
  double pitch = 0;

public:
  void startWalkthrough(const string& sceneFilePath);
  bool onUpdate();

private:
  void endWalkthrough();
  string createGLSLHitTestScene(Scene* scene);
  string createGLSLShadeSurface(Scene* scene);
  void setCamRotation(const Vector3D& camViewDirection);
  };
