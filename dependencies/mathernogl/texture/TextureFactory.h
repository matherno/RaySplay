#pragma once

#include <stdexcept>
#include <GL/glew.h>

#include <mathernogl/utils/Logging.h>
#include <mathernogl/utils/ImageImporter.h>
#include <mathernogl/image/ImageData.h>
#include <mathernogl/texture/Texture.h>

namespace mathernogl {

enum TextureFiltering {
	NEAREST, LINEAR
};

enum TextureWrapping {
	REPEAT, REPEAT_MIRRORED, CLAMP_EDGE, CLAMP_BORDER
};

Texture* createTextureFromFile(const std::string& filePath, bool genMipMaps);
Texture* createTextureFromFile(const std::string& filePath, bool genMipMaps, TextureFiltering filtering, TextureWrapping wrapping);
Texture* createEmptyTexture(uint width, uint height, TextureFiltering filtering = LINEAR, TextureWrapping wrapping = REPEAT, uint bytesPerPixel = 3);
Texture* createEmptyAlphaTexture(uint width, uint height, TextureFiltering filtering = LINEAR, TextureWrapping wrapping = REPEAT);
void updateTexture(Texture* texture);

/*
 * Creates a texture within opengl, returning the ID handle to it
 * if not RGB (rgbFormat), then will read the texture bytes in BGR format
 */
uint createGLTexture(uint glTexType, uint width, uint height, uint bytesPerPixel, bool genMipMaps, TextureFiltering filtering, TextureWrapping wrapping, bool rgbFormat, const byte* byteData);

}
