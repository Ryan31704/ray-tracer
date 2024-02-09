#ifndef MATERIAL_H
#define MATERIAL_H

#include"rtweekend.h"
#include"color.h"
#include"hittable.h"
class hit_record;

class material
{
  public:
    virtual ~material() = default;

    virtual bool scatter(const ray& rIn, const hit_record& record, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
  public:
    lambertian(const color& a) : albedo(a) {}

    bool scatter(const ray& rIn, const hit_record& record, color& attenuation, ray& scattered) const override
    {
      auto scatterDirection = record.normal + randomUnitVector();

      //catch degenerate scatter direction
      if(scatterDirection.nearZero())
        scatterDirection = record.normal;


      scattered = ray(record.p, scatterDirection);
      attenuation = albedo;
      return true;
    }
  private:
    color albedo;
};

class metal : public material
{
  public:
    metal(const color& a) : albedo(a) {}

    bool scatter(const ray& rIn, const hit_record& record, color& attenuation, ray& scattered) const override
    {
      vec3 reflected = reflect(unitVector(rIn.direction()), record.normal);
      scattered = ray(record.p, reflected);
      attenuation = albedo;
      return true;
    }
  private:
    color albedo;
};

#endif