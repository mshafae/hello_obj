
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "drawobject.h"
#include "global.h"
#include "trackball.h"
#include "util.h"

#ifndef CALLBACKS_H
#define CALLBACKS_H

// Forward declaration
// namespace tinyobj {
// struct material_t;
// }

void CheckErrors(std::string desc);

void reshapeFunc(GLFWwindow* window, int w, int h);

void keyboardFunc(GLFWwindow* window, int key, int scancode, int action,
                  int mods);

void clickFunc(GLFWwindow* window, int button, int action, int mods);

void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y);

void Draw(const std::vector<DrawObject>& drawObjects,
          std::vector<tinyobj::material_t>& materials,
          std::map<std::string, GLuint>& textures);

#endif