//owns raytracer and rasterizer and input parser
//initialised with infilename
//should call appropriate GL/GLFW functions to setup but rasterizer does all vbo vao stuff
// move cam here : void moveCam(const glm::vec3 &offset);


#ifndef __RENDERER_H__
#define __RENDERER_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <math.h>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

// Objects
#include "InputParser.h"
#include "RayTracer.h"
#include "Rasterizer.h"
#include "Camera.h"

////////////////////////////////////////////////////////////////////////////////

class Renderer{
  private:
    bool rasterization;
    RayTracer* rt;
    Rasterizer* rast;
    void initInstanceVariables(const InputParser& parser);
  public:
    Renderer();
    Renderer(const InputParser &ip);
    void initialize();
    void update(float appDT);
    void draw();
};

#endif