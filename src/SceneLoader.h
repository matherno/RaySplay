#pragma once

#include <geometries/ObjMeshSet.h>
#include "SceneDef.h"
#include "Scene.h"

/*
*   
*/

class SceneLoader
  {
public:
  static std::shared_ptr<ObjMeshSet> loadSceneFromOBJ(const std::string& filePath);

  static bool loadSceneFromXML(const std::string& filePath, Scene* scene);
  static bool saveSceneToXML(const std::string& filePath, Scene* scene);
  };
