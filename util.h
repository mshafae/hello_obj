#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

#include <string>
#include <cstdio>
#include <cstdlib>

#ifndef UTIL_H
#define UTIL_H
void CheckErrors(std::string desc);
std::string GetBaseDir(const std::string& filepath);
bool FileExists(const std::string& abs_filename);
void CalcNormal(float N[3], float v0[3], float v1[3], float v2[3]);
#endif