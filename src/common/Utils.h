#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace nocg {

unsigned int loadShaders(vector<string> shaderFiles);

// loads texture and returns texture id
unsigned int loadTexture(const string& fileName, bool repeat);

} // namespace nocg