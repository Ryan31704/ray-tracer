#ifndef SPHERE_H
#define SPHERE_H
#include"hittable.h"
#include"vec3.h"

class sphere : public hittable{
  public:
  sphere(point3 _center, double _radius, shared_ptr<material> _material) : center(_center), radius(_radius), mat(_material){}

  bool hit(const ray& r, interval rayT, hit_record& hitRec) const override
  {
    vec3 oc = r.origin() - center;
    auto a = r.direction().lengthSquared();
    auto bHalf = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius*radius;

    auto discriminant = bHalf * bHalf - a*c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    //Find nearest root that lies in acceptable range
    auto root = (-bHalf - sqrtd) / a;
    if(!rayT.surrounds(root))
    {
      root = (-bHalf - sqrtd) / a;
      if(!rayT.surrounds(root))
        return false;

    }
    hitRec.t = root;
    hitRec.p = r.at(hitRec.t);
    vec3 outward_normal = (hitRec.p - center) / radius;
    hitRec.setFaceNormal(r, outward_normal);
    hitRec.mat = mat;
    return true;
  }

  private:
  point3 center;
  double radius;
  shared_ptr<material> mat;
};
#endif