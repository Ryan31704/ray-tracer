//CURRENT PROGRESS
//Ray tracing in one weekend
//Part 10
#include"rtweekend.h"
#include"camera.h"
#include"color.h"
#include"hittable_list.h"
#include"material.h"
#include"sphere.h"

int main()
{
  hittable_list world;
  auto materialGround = make_shared<lambertian>(color(0.8,0.8,0));
  auto materialCenter = make_shared<lambertian>(color(0.2,0.2,0.8));
  auto materialLeft = make_shared<metal>(color(0.8,0.8,0.8));
  auto materialRight = make_shared<metal>(color(0.8,0.2,0.2));

  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, materialGround));
  world.add(make_shared<sphere>(point3( 0.0,    0.1, -1.0),   0.5, materialCenter));
  world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, materialLeft));
  world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, materialRight));
  

  camera cam;

  cam.aspectRatio = 16.0 / 9.0;
  cam.imageWidth  = 400;
  cam.samplesPerPixel = 100;
  cam.maxDepth = 50;
  //cam.reflectance = 0.7;

  cam.render(world);
}