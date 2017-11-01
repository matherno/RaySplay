#pragma once
//
// Created by matherno on 29/10/17.
//


#include <set>
#include "RaySplayConst.h"
#include "Texture.h"

class ResourceManager {
private:
  std::map<string, TexturePtr> loadedTextures;

public:
  /*
   *  Loads the image at the given file path into the returned texture
   *  Will only load a given image once, caching each image loaded by file path
   */
  TexturePtr loadImageTexture(const string& filePath);

  /*
   *  Clears all the cached data in this Resource Manager
   */
  void clearResourcesCache();
};
