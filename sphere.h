#ifndef SPHERE_H
#define SPHERE_H
#include"hittable.h"
#include"vec3.h"

class sphere : hittable{
  public:
  sphere(point3 _center, double _radius) : center(_center), radius(_radius){}

  bool hit(const ray& r, double rayTmin, double rayTmax, hit_record& hitRec) const override
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
    if(root <= rayTmin || rayTmax <= root)
    {
      root = (-bHalf - sqrtd) / a;
      if(root <=rayTmin || rayTmax <= root)
        return false;

    }
    hitRec.t = root;
    hitRec.p = r.at(hitRec.t);
    hitRec.normal = (hitRec.p - center) / radius;
    
    return true;
  }

  private:
  point3 center;
  double radius;
};

#endif