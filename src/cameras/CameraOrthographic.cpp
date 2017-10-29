//
// Created by matherno on 26/08/17.
//

#include "CameraOrthographic.h"

CameraOrthographic::CameraOrthographic() {}

CameraOrthographic::CameraOrthographic(const Vector3D& position, const Vector3D& viewDirection, const Vector3D& viewUp)
    : Camera(position, viewDirection, viewUp) {}

void CameraOrthographic::getScreenRay(const Vector2D& devicePoint, const ViewDef* viewDef, Ray* screenRay) {
  float pixelSize = viewDef->getPixelSize();
  Vector2D rayOrigin;
  rayOrigin.x = devicePoint.x - (viewDef->width / 2.0f);
  rayOrigin.y = (float)viewDef->height - devicePoint.y - (viewDef->height / 2.0f);
  rayOrigin *= pixelSize;

  Vector3D u, v, w;
  getOrthoBasis(&u, &v, &w);
  screenRay->origin = u * rayOrigin.x + v * rayOrigin.y;
  screenRay->origin += position;
  screenRay->direction = viewDirection.getUniform();
}
