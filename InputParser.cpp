#ifndef __INPUTPARSER_CPP__
#define __INPUTPARSER_CPP__
#include "InputParser.h"

using namespace std;
using json = nlohmann::json;

InputParser::InputParser(const std::string &infileName, int frameWidth, int frameHeight) 
  : frameWidth(frameWidth), frameHeight(frameHeight)
{
  std::string fileExt = infileName.substr(infileName.find_last_of(".") + 1);
  if(fileExt != "scene") {
    cerr << "Input file format not recognised. Please input a '.scene' file."  << endl ;
    exit(EXIT_FAILURE);
  }
  parse(infileName);
};

void InputParser::initializeRenderingMethod(string type) {
  // checking rendering type
  if(type == "raytracing") rasterize = false;
  else if(type == "rasterization") rasterize = true;
  else {
    cerr << "Rendering method type not recognised. \n Please mention 'type rasterization' or 'type raytracing' in your input .scene file."  << endl ;
    exit(EXIT_FAILURE);
  }
}

void InputParser::initializeView(string type) {
  // checking view type
  if(type == "perspective") perspective = true;
  else if(type == "orthographic") perspective = false;
  else {
    cerr << "View type not recognised. \n Please mention 'view perspective' or 'view orthographic' in your input .scene file."  << endl ;
    exit(EXIT_FAILURE);
  }
}

void InputParser::parse(const string& fn) {
  std::cout << "Parsing: " << fn << std::endl;

  std::ifstream ifs(fn);
  if(!ifs) {
    exit(EXIT_FAILURE);
  }

  // parsing
  std::string line;
  while(getline(ifs, line)) {

    std::istringstream iss(line);
    std::string tag;
    iss >> tag;

    // add handling type
    if(tag == "wireframe") {
      wireframe = true;
    }
    else if(tag == "type") {
      string t;
      iss >> t;
      initializeRenderingMethod(t);
    }
    else if(tag == "view") {
      string t;
      iss >> t;
      initializeView(t);
    }
    else if(tag == "objects") {
      string fn;
      iss >> fn;
      cout << "Parsing: " << fn << endl;
      std::ifstream ifs(fn);
      json objects;
      ifs >> objects;
      parseObjectsJSON(objects);
    }
  }
}

void InputParser::parseObjectsJSON(json& j) {
  // initializing camera
  if(j.contains("camera")) {
    json cam = j["camera"];
    camera = new Camera(cam, perspective, rasterize, frameWidth, frameHeight);
  } else {
    cerr << "No camera definition found."  << endl ;
    exit(EXIT_FAILURE);
  }

  // parsing spheres
  if(j.contains("spheres")) {
    for(json s : j["spheres"]) {
      Sphere* sp = new Sphere(s);
      //.getMesh() and emplace into meshes
      models.emplace_back(sp);
    }
  }

  // parsing planes
  if(j.contains("planes")) {
    for(json p : j["planes"]) {
      Plane* pl = new Plane(p);
      if(rasterize) {
        // parsing T
        glm::vec3 t = glm::vec3(p["T"][0], p["T"][1], p["T"][2]);
        glm::mat4 translationM = glm::translate(glm::mat4(1.0f), t);

        // parsing R
        float angleDeg = p["R"][0];
        glm::vec3 axis = glm::vec3(p["R"][1], p["R"][2], p["R"][3]);
        glm::mat4 rotationM = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), axis);

        // parsing S
        glm::vec3 s = glm::vec3(p["S"][0], p["S"][1], p["S"][2]);
        glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), s);
        meshes.emplace_back(pl->getMesh(translationM, rotationM, scaleM));
      }
      else {
        models.emplace_back(pl);
      }
    }
  }

  // parsing lights
  if(j.contains("lights")) {
    for(json l : j["lights"]) {
      string type = l["type"];
      if(type == "point" || type == "spot") {
        PositionalLight* posLight = new PositionalLight(l);
        posLights.emplace_back(posLight);
      }
      else if(type == "dir") {
        DirectionalLight* dirLight = new DirectionalLight(l);
        dirLights.emplace_back(dirLight);
      }
    }
  }

  //parsing meshes
  if(j.contains("meshes")) {
    for(json m: j["meshes"]) {
      Mesh* mesh = new Mesh(m);
      meshes.emplace_back(mesh);
    }
  }

  //parsing particle system
  if(j.contains("particle_system")) {
    part_sys = j["particle_system"];
  }
};

#endif