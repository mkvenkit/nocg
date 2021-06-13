#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include <string>

using std::string;

namespace nocg {

// loads texture and returns texture id
unsigned int loadTexture(const string& fileName);

} // namespace nocg