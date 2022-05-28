#ifndef __RENDERER_CPP__
#define __RENDERER_CPP__
#include "Renderer.h"

using namespace std;

Renderer::Renderer(const InputParser &ip)
  : rasterization(ip.shouldRasterize())
{
  cout << "Creating Renderer" << endl;
  if(rasterization) {
    rast = new Rasterizer(ip);
    rt = nullptr;
  }
  else {
    rt = new RayTracer(ip);
    rast = nullptr;
  }
}

void Renderer::initialize() {
  if(rasterization) {
    rast->initialize();
  }
  else {
    rt->initialize();
  }
}

void Renderer::update(float appDT) {
  if(rasterization) {
    rast->update(appDT);
  }
  // else {
  //   rt->update();
  // }
}

void Renderer::draw() {
  if(rasterization) {
    rast->draw();
  }
  else {
    rt->draw();
  }
}

#endif