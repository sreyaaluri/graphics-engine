////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine that
///        repeatedly generates a framebuffer and displays it.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>

// GL / GLM
#include "GLInclude.h"

// SOIL
#include <SOIL2/SOIL2.h>

//Objects / Other
#include "CompileShaders.h"
#include "InputParser.h"
#include "Camera.h"
#include "Renderer.h"

// Global variables - avoid these

// Window
int g_width{1360};
int g_height{768};
int g_window{0};
GLuint g_program{0};
GLuint g_vao{0}; ///< Vertex Array Object
GLuint g_vbo{0}; ///< Vertex Buffer Object

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{
    std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

// Time
float deltaTime{0.0f};	// Time between current frame and last frame
float lastFrame{0.0f};  // Time of last frame
float g_target_frame_time{0.017f};
float g_app_time_factor{0.03f};

//Cursor
float lastX = g_width/2;
float lastY = g_height/2;
bool firstMouse = true;

// Input Parser
InputParser* g_parserPtr;

// Renderer
Renderer* g_renderer;

// Camera
Camera* g_camera;

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void initialize(GLFWwindow *window)
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
  g_renderer->initialize();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.
void resize(GLFWwindow *window, int _w, int _h)
{
  g_width = _w;
  g_height = _h;

  // Viewport
  glfwGetFramebufferSize(window, &g_width, &g_height);
  glViewport(0, 0, g_width, g_height);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Update function for single time-step frame
void update(GLFWwindow *window, double deltaTime)
{
  g_renderer->update(g_app_time_factor*deltaTime);
  //this is this it?
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void draw(GLFWwindow *window, double currentTime)
{
  using namespace std::chrono;

  g_renderer->draw();

  //////////////////////////////////////////////////////////////////////////////
  // Record frame time
  high_resolution_clock::time_point time = high_resolution_clock::now();
  g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
  g_frameTime = time;
  g_framesPerSecond = 1.f / (g_delay + g_frameRate);
  printf("FPS: %6.2f\n", g_framesPerSecond);
  
  // Calculate delta time
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;  
}

void loop(GLFWwindow *window)
{
  while (!glfwWindowShouldClose(window))
  {
    draw(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
    update(window, deltaTime);
    
    // frame limiting
    while(deltaTime < g_target_frame_time){
      deltaTime = glfwGetTime() - lastFrame; // recalculating delta time
    }
  }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    std::cout << "Destroying window: " << g_window << std::endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  
  g_camera->processKeyInput(key, action, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if(firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; 
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.05f;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  g_camera->processMouseInput(xoffset, yoffset);
}

void error_callback(int error, const char *description)
{
  std::cout << error << ":" << description << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int main(int _argc, char **_argv)
{
  if(_argc!=2){
    std::cout << "This program takes an input file. To run: " << _argv[0] << "  <input file name (.scene file)>" << std::endl;
    return 0;
  }

  g_parserPtr = new InputParser(_argv[1], g_width*2, g_height*2);
  g_renderer = new Renderer(*g_parserPtr);
  g_camera = g_parserPtr->getCamera();

  glfwSetErrorCallback(error_callback);
  std::cout << "Initializing GLFW\n"
            << std::endl;
  if (!glfwInit())
  {
    std::cout << "GLFW Initialization Failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  if(g_parserPtr->rasterize) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  }

  GLFWwindow *window = glfwCreateWindow(g_width, g_height, "Spiderling", NULL, NULL);
  if (!window)
  {
    std::cout << "GLFWWindow Initialization Failed" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwGetFramebufferSize(window, &g_width, &g_height);
  glfwMakeContextCurrent(window);
  glViewport(0, 0, g_width, g_height);
  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK)
  {
    exit(EXIT_FAILURE);
  }
  glfwSwapInterval(1);

  initialize(window);

  //////////////////////////////////////////////////////////////////////////////
  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetFramebufferSizeCallback(window, resize);

  std::cout << "Starting Application" << std::endl;
  loop(window);

  //////////////////////////////////////////////////////////////////////////////
  // End Application
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);

  return 0;
}

#pragma clang diagnostic pop
