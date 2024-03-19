

#ifndef OBJUTIL_H
#define OBJUTIL_H

// namespace tinyobj {
// struct material_t;
// }

bool LoadObjAndConvert(float bmin[3], float bmax[3],
                       std::vector<DrawObject>* drawObjects,
                       std::vector<tinyobj::material_t>& materials,
                       std::map<std::string, GLuint>& textures,
                       const char* filename);

#endif