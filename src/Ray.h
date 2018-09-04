#pragma once
//
// Created by matherno on 31/07/17.
//

#include <mathernogl/MathernoGL.h>
#include "RaySplayConst.h"

struct Ray {

  static Ray create(const Vector3D& origin, const Vector3D& direction){
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
  }

  static Ray createFromPoints(const Vector3D& point1, const Vector3D& point2){
    Ray ray;
    ray.origin = point1;
    ray.direction = point2 - point1;
    ray.direction.makeUniform();
    return ray;
  }

  Vector3D origin;
  Vector3D direction;
  int depth = 0;

  Vector3D getPosition(double t) const { return origin + direction * t; }

  static double projectPointOntoRay(const Ray* ray, Vector3D point){
    Vector3D rayOriginToPoint = point - ray->origin;
    double cosAngle = mathernogl::dotProduct(rayOriginToPoint.getUniform(), ray->direction);
    return cosAngle * rayOriginToPoint.magnitude();
  }
};
