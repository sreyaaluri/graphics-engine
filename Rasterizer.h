#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

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
#include "Material.h"
#include "CompileShaders.h"
#include "Mesh.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "ParticleSystem.h"

////////////////////////////////////////////////////////////////////////////////

class Rasterizer{
  private:
    bool wireframe = false;
    Camera* camera;
    ParticleSystem* partSys;
    GLuint triangleProgram;
    GLuint pointProgram;
    std::vector<Mesh*> meshes;
    std::vector<PositionalLight*> posLights;
    std::vector<DirectionalLight*> dirLights;
    void sendMeshVertexData();
    void sendLightData();
    void sendMeshUniforms(const Mesh& m);
    void sendPartSysUniforms();
  public:
    Rasterizer();
    Rasterizer(const InputParser& parser);

    void initialize();
    void update(float appDT);
    void draw();
    void toggleWireFrame() { wireframe = !wireframe; };
};

#endif