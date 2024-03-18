#include "global.h"

std::vector<DrawObject> gDrawObjects;

int width = 768;
int height = 768;

double prevMouseX, prevMouseY;
bool mouseLeftPressed;
bool mouseMiddlePressed;
bool mouseRightPressed;
float curr_quat[4];
float prev_quat[4];
float eye[3], lookat[3], up[3];
bool g_show_wire = true;
bool g_cull_face = false;

GLFWwindow* window;
