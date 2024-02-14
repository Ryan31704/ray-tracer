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
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& rIn, const hit_record& record, color& attenuation, ray& scattered) const override
    {
      vec3 reflected = reflect(unitVector(rIn.direction()), record.normal);
      scattered = ray(record.p, reflected + fuzz *randomUnitVector());
      attenuation = albedo;
      return (dot(scattered.direction(), record.normal) > 0);
    }
  private:
    color albedo;
    double fuzz;
};

class dielectric : public material
{
  public:
    dielectric(double indexOfRefraction) : indexRefraction(indexOfRefraction) {}

    bool scatter(const ray& rIn, const hit_record& record, color& attenuation, ray& scattered) const override
    {
      attenuation = color(1.0,1.0,1.0);
      double refractionRatio = record.frontFace ? (1.0/indexRefraction) : indexRefraction;

      vec3 unitDirection = unitVector(rIn.direction());

      double cos_theta = fmin(dot(-unitDirection, record.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refractionRatio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refractionRatio) > randomDouble())
            direction = reflect(unitDirection, record.normal);
        else
            direction = refract(unitDirection, record.normal, refractionRatio);

        scattered = ray(record.p, direction);
      return true;
    }

  private:
    double indexRefraction;
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
      auto r0 = (1-ref_idx) / (1+ref_idx);
      r0 = r0*r0;
      return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

#endif