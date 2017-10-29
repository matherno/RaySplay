//
// Created by matherno on 26/08/17.
//

#include "Camera.h"

Camera::Camera()
    : position(0, 0, 0), viewDirection(0, 0, -1), viewUp(0, 1, 0) {}

Camera::Camera(const Vector3D& position, const Vector3D& viewDirection, const Vector3D& viewUp)
    : position(position), viewDirection(viewDirection), viewUp(viewUp) {}

Vector3D Camera::getPosition() const {
  return position;
}

void Camera::setPosition(const Vector3D& position) {
  this->position = position;
  orthoBasisValid = false;
}

Vector3D Camera::getViewDirection() const {
  return viewDirection;
}

void Camera::setViewDirection(const Vector3D& viewDirection) {
  this->viewDirection = viewDirection;
  orthoBasisValid = false;
}

Vector3D Camera::getViewUp() const {
  return viewUp;
}

void Camera::setViewUp(const Vector3D& viewUp) {
  this->viewUp = viewUp;
  orthoBasisValid = false;
}

void Camera::lookAt(Vector3D lookPoint) {
  setViewDirection(lookPoint - position);
}

void Camera::calculateOrthoBasis() {
  w = viewDirection * -1;
  w.makeUniform();
  u = mathernogl::crossProduct(viewUp, w);
  u.makeUniform();
  v = mathernogl::crossProduct(w, u);
  orthoBasisValid = true;
}

void Camera::getOrthoBasis(Vector3D* u, Vector3D* v, Vector3D* w) {
  if(!orthoBasisValid){
    calculateOrthoBasis();
  }
  *u = this->u;
  *v = this->v;
  *w = this->w;
}
