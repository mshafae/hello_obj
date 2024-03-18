
#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

typedef struct {
  GLuint vb_id;  // vertex buffer id
  int numTriangles;
  size_t material_id;
} DrawObject;

#endif