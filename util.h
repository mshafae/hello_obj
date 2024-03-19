
#ifndef UTIL_H
#define UTIL_H
void CheckErrors(std::string desc);
std::string GetBaseDir(const std::string& filepath);
bool FileExists(const std::string& abs_filename);
#endif