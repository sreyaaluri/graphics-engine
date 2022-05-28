#ifndef __INPUTPARSER_H__
#define __INPUTPARSER_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp>

// Objects
#include "Camera.h"
#include "Material.h"
#include "PositionalLight.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "Sphere.h"
#include "Plane.h"
#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;
using json = nlohmann::json;

class InputParser{
  private:
    std::vector<Mesh*> meshes;
    std::vector<Model*> models;
    std::vector<PositionalLight*> posLights;
    std::vector<DirectionalLight*> dirLights;
    Camera* camera;
    json part_sys;

    void parse(const string& fn);
    void parseObjectsJSON(json& objects);
    void initializeRenderingMethod(string type);
    void initializeView(string type);

  public:
    InputParser();
    InputParser(const std::string &infileName, int frameWidth, int frameHeight);

    int frameWidth;
    int frameHeight;
    bool rasterize = true;
    bool wireframe = false;
    bool perspective = true;
    bool shouldRasterize() const { return rasterize; };
    Camera* getCamera() const { return camera; };
    const json& getPartSysJSON() const { return part_sys; };
    std::vector<Mesh*> getMeshes() const { return meshes; };
    std::vector<Model*> getModels() const { return models; };
    std::vector<DirectionalLight*> getDirLights() const { return dirLights; };
    std::vector<PositionalLight*> getPosLights() const { return posLights; };
};

#endif