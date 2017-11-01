//
// Created by matherno on 29/10/17.
//

#include <textures/ImageTexture.h>
#include "ResourceManager.h"

TexturePtr ResourceManager::loadImageTexture(const string& filePath) {
  using namespace mathernogl;
  auto textureMapIter = loadedTextures.find(filePath);
  if (textureMapIter == loadedTextures.end()) {
    ImageTexture* texture = nullptr;
    try {
      std::unique_ptr<Image> image = Image::load(filePath);
      if (image) {
        Image::ColourVector* pixels = new Image::ColourVector();
        image->readColourData(pixels);
        uint width = image->getWidth();
        uint height = image->getHeight();
        if (pixels->size() >= width * height) {
          texture = new ImageTexture(std::move(ColourMapPtr(pixels)), width, height);
        }
      }

    }
    catch (const std::runtime_error& error) {
      texture = nullptr;
    }

    if (texture) {
      TexturePtr texturePtr = TexturePtr(texture);
      loadedTextures[filePath] = texturePtr;
      mathernogl::logInfo("Loaded Image Texture: '" + filePath + "'");
      return texturePtr;
    }
    else {
      logWarning("Couldn't load image! " + filePath);
      return nullptr;
    }
  }
  else{
    return textureMapIter->second;
  }
}

void ResourceManager::clearResourcesCache() {
  loadedTextures.clear();
}
