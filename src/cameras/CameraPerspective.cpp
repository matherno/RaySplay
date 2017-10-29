//
// Created by matherno on 26/08/17.
//

#include "CameraPerspective.h"

CameraPerspective::CameraPerspective() {}

CameraPerspective::CameraPerspective(const Vector3D& position, const Vector3D& viewDirection, const Vector3D& viewUp)
    : Camera(position, viewDirection, viewUp) {}

void CameraPerspective::setViewPlaneOffset(float viewPlaneOffset) {
  this->viewPlaneOffset = viewPlaneOffset;
}

void CameraPerspective::getScreenRay(const Vector2D& devicePoint, const ViewDef* viewDef, Ray* screenRay) {
  float pixelSize = viewDef->getPixelSize();
  Vector2D rayOrigin;
  rayOrigin.x = devicePoint.x - (viewDef->width / 2.0f);
  rayOrigin.y = (float)viewDef->height - devicePoint.y - (viewDef->height / 2.0f);
  rayOrigin *= pixelSize;

  Vector3D u, v, w;
  getOrthoBasis(&u, &v, &w);
  screenRay->origin = u * rayOrigin.x + v * rayOrigin.y;
  screenRay->origin += position;
  screenRay->origin += viewDirection.getUniform() * viewPlaneOffset;

  screenRay->direction = (screenRay->origin - position).getUniform();
}

