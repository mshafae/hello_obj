
#include "callbacks.h"
#include <tiny_obj_loader.h>

void reshapeFunc(GLFWwindow* window, int w, int h) {
  int fb_w, fb_h;
  // Get actual framebuffer size.
  glfwGetFramebufferSize(window, &fb_w, &fb_h);

  glViewport(0, 0, fb_w, fb_h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, 0.01f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  width = w;
  height = h;
}

void keyboardFunc(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  (void)window;
  (void)scancode;
  (void)mods;
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    // Move camera
    float mv_x = 0, mv_y = 0, mv_z = 0;
    if (key == GLFW_KEY_K)
      mv_x += 1;
    else if (key == GLFW_KEY_J)
      mv_x += -1;
    else if (key == GLFW_KEY_L)
      mv_y += 1;
    else if (key == GLFW_KEY_H)
      mv_y += -1;
    else if (key == GLFW_KEY_P)
      mv_z += 1;
    else if (key == GLFW_KEY_N)
      mv_z += -1;
    // camera.move(mv_x * 0.05, mv_y * 0.05, mv_z * 0.05);
    // Close window
    if (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) {
      glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_W) {
      // toggle wireframe
      g_show_wire = !g_show_wire;
    }

    if (key == GLFW_KEY_C) {
      // cull option
      g_cull_face = !g_cull_face;
    }

    // init_frame = true;
  }
}

void clickFunc(GLFWwindow* window, int button, int action, int mods) {
  (void)window;
  (void)mods;
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      mouseLeftPressed = true;
      trackball(prev_quat, 0.0, 0.0, 0.0, 0.0);
    } else if (action == GLFW_RELEASE) {
      mouseLeftPressed = false;
    }
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      mouseRightPressed = true;
    } else if (action == GLFW_RELEASE) {
      mouseRightPressed = false;
    }
  }
  if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
    if (action == GLFW_PRESS) {
      mouseMiddlePressed = true;
    } else if (action == GLFW_RELEASE) {
      mouseMiddlePressed = false;
    }
  }
}

void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y) {
  (void)window;
  float rotScale = 1.0f;
  float transScale = 2.0f;

  if (mouseLeftPressed) {
    trackball(prev_quat, rotScale * (2.0f * prevMouseX - width) / (float)width,
              rotScale * (height - 2.0f * prevMouseY) / (float)height,
              rotScale * (2.0f * mouse_x - width) / (float)width,
              rotScale * (height - 2.0f * mouse_y) / (float)height);

    add_quats(prev_quat, curr_quat, curr_quat);
  } else if (mouseMiddlePressed) {
    eye[0] -= transScale * (mouse_x - prevMouseX) / (float)width;
    lookat[0] -= transScale * (mouse_x - prevMouseX) / (float)width;
    eye[1] += transScale * (mouse_y - prevMouseY) / (float)height;
    lookat[1] += transScale * (mouse_y - prevMouseY) / (float)height;
  } else if (mouseRightPressed) {
    eye[2] += transScale * (mouse_y - prevMouseY) / (float)height;
    lookat[2] += transScale * (mouse_y - prevMouseY) / (float)height;
  }

  // Update mouse point
  prevMouseX = mouse_x;
  prevMouseY = mouse_y;
}

void Draw(const std::vector<DrawObject>& drawObjects,
                 std::vector<tinyobj::material_t>& materials,
                 std::map<std::string, GLuint>& textures) {
  glPolygonMode(GL_FRONT, GL_FILL);
  if (g_cull_face) {
    glPolygonMode(GL_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_BACK, GL_FILL);
  }

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);
  GLsizei stride = (3 + 3 + 3 + 2) * sizeof(float);
  for (size_t i = 0; i < drawObjects.size(); i++) {
    DrawObject o = drawObjects[i];
    if (o.vb_id < 1) {
      continue;
    }

    glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    if ((o.material_id < materials.size())) {
      std::string diffuse_texname = materials[o.material_id].diffuse_texname;
      if (textures.find(diffuse_texname) != textures.end()) {
        glBindTexture(GL_TEXTURE_2D, textures[diffuse_texname]);
      }
    }
    glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
    glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
    glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
    glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

    glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
    CheckErrors("drawarrays");
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  // draw wireframe
  if (g_show_wire) {
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);

    glColor3f(0.0f, 0.0f, 0.4f);
    for (size_t i = 0; i < drawObjects.size(); i++) {
      DrawObject o = drawObjects[i];
      if (o.vb_id < 1) {
        continue;
      }

      glBindBuffer(GL_ARRAY_BUFFER, o.vb_id);
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glVertexPointer(3, GL_FLOAT, stride, (const void*)0);
      glNormalPointer(GL_FLOAT, stride, (const void*)(sizeof(float) * 3));
      glColorPointer(3, GL_FLOAT, stride, (const void*)(sizeof(float) * 6));
      glTexCoordPointer(2, GL_FLOAT, stride, (const void*)(sizeof(float) * 9));

      glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
      CheckErrors("drawarrays");
    }
  }
}
