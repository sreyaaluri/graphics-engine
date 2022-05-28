#ifndef __MESH_CPP__
#define __MESH_CPP__
#include "Mesh.h"

using namespace std;

Mesh::Mesh(const nlohmann::json &mesh) {
  // mesh variables (add vector of triangles)
  std::vector<glm::vec4> positions;
  std::vector<glm::vec4> normals;
  std::vector<glm::vec2> textures;
  glm::mat4 translationM;
  glm::mat4 rotationM;
  glm::mat4 scaleM;

  // parsing T
  glm::vec3 t = glm::vec3(mesh["T"][0], mesh["T"][1], mesh["T"][2]);
  translationM = glm::translate(glm::mat4(1.0f), t);

  // parsing R
  float angleDeg = mesh["R"][0];
  glm::vec3 axis = glm::vec3(mesh["R"][1], mesh["R"][2], mesh["R"][3]);
  rotationM = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), axis);

  // parsing S
  glm::vec3 s = glm::vec3(mesh["S"][0], mesh["S"][1], mesh["S"][2]);
  scaleM = glm::scale(glm::mat4(1.0f), s);

  // initializing Model matrix
  M = translationM * rotationM * scaleM;
  
  // parsing obj file
  string fn = mesh["model"];
  std::cout << "Parsing: " << fn << std::endl;

  std::ifstream ifs(fn);
  std::string line;
  while(getline(ifs, line)) {

    std::istringstream iss(line);
    std::string tag;
    iss >> tag;

    if(tag == "mltlib") {
      string fn;
      iss >> fn;
      mtl = new Material(fn); // initializing material 
    }
    if(tag == "v") {
      glm::vec4 p;
      iss >> p.x >> p.y >> p.z;
      p.w = 1;
      positions.emplace_back(p);
    }
    if(tag == "vt") {
      glm::vec2 t;
      iss >> t.x >> t.y;
      textures.emplace_back(t);
    }
    else if(tag == "vn") {
      glm::vec4 n;
      iss >> n.x >> n.y >> n.z;
      n.w = 0;
      normals.emplace_back(n);
    }
    else if(tag == "f") {
      for(size_t i = 0; i < 3; ++i) { // initializing vertices
        std::string vert;
        iss >> vert;
        size_t p, t, n;
        sscanf(vert.c_str(), "%zu/%zu/%zu", &p, &t, &n);
        vertices.emplace_back(positions[p-1], normals[n-1], textures[t-1]);
        // create triangles here
      }
    }
  }
}

void Mesh::initialize() {
  //requesting Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //generate vertex buffer object
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

  //specifying layout of data using vertex attribute pointer
  glEnableVertexAttribArray(0);                                                       // 1st attribute in VBO (position)
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), (char *)NULL + 0);                            // detailing 1st attribute location

  glEnableVertexAttribArray(1);                                                       // 2nd attribute in VBO (normal)
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), (char *)NULL + sizeof(glm::vec4));            // detailing 2nd attribute location

  glEnableVertexAttribArray(2);                                                       // 3rd attribute in VBO (texture)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                        sizeof(vertex), (char *)NULL + 2 * sizeof(glm::vec4));        // detailing 3rd attribute location

  //loading textures
  mtl->initialize();
}

void Mesh::draw() {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);
  glDisable(GL_TEXTURE_2D);
}

#endif