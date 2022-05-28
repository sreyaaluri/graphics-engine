#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <math.h>
#include <fstream>

// GL
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

// Objects
#include "Ray.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"
#include "Plane.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"
#include "InputParser.h"

////////////////////////////////////////////////////////////////////////////////

class RayTracer{
  private:
    std::unique_ptr<glm::vec4[]> g_frame; ///< Framebuffer
    bool perspective;
    int frameWidth;
    int frameHeight;
    Camera* camera;
    std::vector<Model*> models;
    std::vector<PositionalLight*> posLights;
    std::vector<DirectionalLight*> dirLights;
    glm::vec4 getPixelColor(float i, float j);
    void initInstanceVariables(const InputParser& parser);
    const glm::vec3 getColorPerPosLight(const PositionalLight& posLight, const Model& model, const CollisionDet& colDet) const;
    const glm::vec3 getColorPerDirLight(const DirectionalLight& dirLight, const Model& model, const CollisionDet& colDet) const;

  public:
    RayTracer();
    RayTracer(const InputParser &ip);
    ~RayTracer();

    void initialize();
    void draw();
};

#endif