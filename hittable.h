#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    bool frontFace;
    void setFaceNormal(const ray& r, const vec3& outwardNormal)
    {
      frontFace = dot(r.direction(), outwardNormal) < 0;
      normal = frontFace ? outwardNormal  : -outwardNormal;
    }
};

class hittable {
  public:
  virtual ~hittable() = default;

  virtual bool hit(const ray& r, interval rayT, hit_record& hitRec) const = 0;
};

#endif