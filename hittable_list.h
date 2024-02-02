#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
  public:
  std::vector<shared_ptr<hittable>> objects;

  hittable_list(){}
  hittable_list(shared_ptr<hittable> object){ add(object);  }

  void clear() { objects.clear(); }

  void add(shared_ptr<hittable> object) { objects.push_back(object); }
  bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override 
  {
    hit_record tempRec;
    bool hitAnything = false;
    auto closestSoFar = ray_tmax;

    for (const auto& object : objects)
    {
      if (object->hit(r, ray_tmin, closestSoFar, tempRec)) 
      {
        hitAnything = true;
        closestSoFar = tempRec.t;
        rec = tempRec;
      }
    }
    return hitAnything;
  }
};

#endif