#pragma once

#include "SceneDef.h"

/*
*   
*/

typedef std::function<void(GeometryPtr object)> AddGeometryFunc;

class SceneLoader
  {
public:
  static void loadSceneFromOBJ(const std::string& filePath, AddGeometryFunc addGeometry);
  };
