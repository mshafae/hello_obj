//
// Simple .obj viewer(vertex only)
//
#include <GL/glew.h>

#include <iostream>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

#define TINYOBJLOADER_IMPLEMENTATION
// TINYOBJLOADER_USE_MAPBOX_EARCUT: Enable better triangulation. Requires C++11
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include <tiny_obj_loader.h>

#include "trackball.h"

// #ifdef __clang__
// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Weverything"
// #endif
//
// #ifdef __clang__
// #pragma clang diagnostic pop
// #endif

#include "callbacks.h"
#include "drawobject.h"
#include "global.h"
#include "objutil.h"
#include "timerutil.h"

static void Init() {
  trackball(curr_quat, 0, 0, 0, 0);

  eye[0] = 0.0f;
  eye[1] = 0.0f;
  eye[2] = 3.0f;

  lookat[0] = 0.0f;
  lookat[1] = 0.0f;
  lookat[2] = 0.0f;

  up[0] = 0.0f;
  up[1] = 1.0f;
  up[2] = 0.0f;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Needs input.obj\n" << std::endl;
    return 0;
  }

  Init();

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW." << std::endl;
    return -1;
  }

  window = glfwCreateWindow(width, height, "Obj viewer", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to open GLFW window. " << std::endl;
    glfwTerminate();
    return 1;
  }

  std::cout << "W : Toggle wireframe\n";
  std::cout << "C : Toggle face culling\n";
  // std::cout << "K, J, H, L, P, N : Move camera\n";
  std::cout << "Q, Esc : quit\n";

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Callback
  glfwSetWindowSizeCallback(window, reshapeFunc);
  glfwSetKeyCallback(window, keyboardFunc);
  glfwSetMouseButtonCallback(window, clickFunc);
  glfwSetCursorPosCallback(window, motionFunc);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW." << std::endl;
    return -1;
  }

  reshapeFunc(window, width, height);

  float bmin[3], bmax[3];
  std::vector<tinyobj::material_t> materials;
  std::map<std::string, GLuint> textures;
  if (false == LoadObjAndConvert(bmin, bmax, &gDrawObjects, materials, textures,
                                 argv[1])) {
    return -1;
  }

  float maxExtent = 0.5f * (bmax[0] - bmin[0]);
  if (maxExtent < 0.5f * (bmax[1] - bmin[1])) {
    maxExtent = 0.5f * (bmax[1] - bmin[1]);
  }
  if (maxExtent < 0.5f * (bmax[2] - bmin[2])) {
    maxExtent = 0.5f * (bmax[2] - bmin[2]);
  }

  while (glfwWindowShouldClose(window) == GL_FALSE) {
    glfwPollEvents();
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // camera & rotate
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat mat[4][4];
    gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up[0],
              up[1], up[2]);
    build_rotmatrix(mat, curr_quat);
    glMultMatrixf(&mat[0][0]);

    // Fit to -1, 1
    glScalef(1.0f / maxExtent, 1.0f / maxExtent, 1.0f / maxExtent);

    // Centerize object.
    glTranslatef(-0.5 * (bmax[0] + bmin[0]), -0.5 * (bmax[1] + bmin[1]),
                 -0.5 * (bmax[2] + bmin[2]));

    Draw(gDrawObjects, materials, textures);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
}
