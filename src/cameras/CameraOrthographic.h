#pragma once
//
// Created by matherno on 26/08/17.
//


#include <Camera.h>

class CameraOrthographic : public Camera {
public:
  CameraOrthographic();
  CameraOrthographic(const Vector3D& position, const Vector3D& viewDirection, const Vector3D& viewUp);

  virtual void getScreenRay(const Vector2D& devicePoint, const ViewDef* viewDef, Ray* screenRay) override;
};
