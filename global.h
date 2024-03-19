#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

#include <vector>

#include "drawobject.h"

#ifndef GLOBALS_H
#define GLOBALS_H
extern std::vector<DrawObject> gDrawObjects;

extern int width;
extern int height;

extern double prevMouseX, prevMouseY;
extern bool mouseLeftPressed;
extern bool mouseMiddlePressed;
extern bool mouseRightPressed;
extern float curr_quat[4];
extern float prev_quat[4];
extern float eye[3], lookat[3], up[3];
extern bool g_show_wire;
extern bool g_cull_face;

extern GLFWwindow* window;
#endif