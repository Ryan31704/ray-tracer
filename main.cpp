//CURRENT PROGRESS
//Ray tracing in one weekend
//Part 14 final render
#include"rtweekend.h"
#include"camera.h"
#include"color.h"
#include"hittable_list.h"
#include"material.h"
#include"sphere.h"

int main()
{
  hittable_list world;
  /*
  auto materialGround = make_shared<lambertian>(color(0.8,0.8,0));
  auto materialCenter = make_shared<lambertian>(color(0.2,0.2,0.8));
  auto materialLeft = make_shared<dielectric>(1.5);
  auto materialRight = make_shared<metal>(color(1.0,0.2,0.2), 0.8);

  //standard scene
  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, materialGround));
  world.add(make_shared<sphere>(point3( 0.0,    0.1, -1.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, materialLeft));
  world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, materialRight));
  */
  
  /*haha funny scene
  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, materialGround));
  world.add(make_shared<sphere>(point3( 0.0,    0.2, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.3, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.4, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.5, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.5, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.6, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.7, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.8, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    0.9, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.0, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.1, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.2, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.3, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.4, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.5, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.6, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3( 0.0,    1.7, -3.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3(-0.5,    0.0, -3.0),   0.5, materialLeft));
  world.add(make_shared<sphere>(point3( 0.5,    0.0, -3.0),   0.5, materialRight));*/
  
  auto groundMaterial = make_shared<lambertian>(color(0.5,0.5,0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, groundMaterial));
  double sphereRadius = 0.2;
  for(int a = -11; a < 11; a++)
  {
    for(int b = -11; b < 11; b++)
    {
      auto chooseMat = randomDouble();
      point3 center(a + 0.9 * randomDouble(), sphereRadius , b + 0.9 * randomDouble());

      if((center - point3(4, sphereRadius, 0)).length() > 0.9)
      {
        shared_ptr<material> sphereMaterial;

        if(chooseMat < 0.8)
        {
          //dissuse 
          auto albedo = color::random() * color::random();
          sphereMaterial = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, sphereRadius, sphereMaterial));
        }
        else if(chooseMat < 0.95)
        {
          //metal
          auto albedo = color::random(0.5,1);
          auto fuzz = randomDouble(0, 0.5);
          sphereMaterial = make_shared<metal>(albedo,fuzz);
          world.add(make_shared<sphere>(center,0.2,sphereMaterial));
        }
        else
        {
          sphereMaterial = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));


  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imageWidth  = 1200;
  cam.samplesPerPixel = 10;
  cam.maxDepth = 50;

  cam.vFov = 20;
  cam.lookfrom = point3(13,2,3);
  cam.lookat   = point3(0,0,0);
  cam.vup      = vec3(0,1,0); 
  cam.defocusAngle = 0.6;
  cam.focusDist = 10.0;

  cam.render(world);
}