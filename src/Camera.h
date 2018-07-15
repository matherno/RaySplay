//
// Created by matherno on 26/08/17.

#pragma once

#include "RaySplayConst.h"
#include "Ray.h"
#include "ViewDef.h"

class Camera {
protected:
  Vector3D position;
  Vector3D viewDirection;
  Vector3D viewUp;

private:
  bool orthoBasisValid = false;
  Vector3D u, v, w;

public:
  Camera();
  Camera(const Vector3D& position, const Vector3D& viewDirection, const Vector3D& viewUp);
  virtual ~Camera() {};

  Vector3D getPosition() const;
  void setPosition(const Vector3D& position);
  Vector3D getViewDirection() const;
  void setViewDirection(const Vector3D& viewDirection);
  Vector3D getViewUp() const;
  void setViewUp(const Vector3D& viewUp);
  void lookAt(Vector3D lookPoint);

  //  returns a world space screen ray which should be traced into the world, to retrieve the colour to be viewed at the given devicePoint
  //  device space starting (0,0) at top-left
  virtual void getScreenRay(const Vector2D& devicePoint, const ViewDef* config, Ray* screenRay) = 0;

protected:
  void calculateOrthoBasis();
  void getOrthoBasis(Vector3D* u, Vector3D* v, Vector3D* w);
};

