#pragma once

#include <string>
#include <vector>

#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using std::string;
using std::vector;

class Render3D
{
public:

    Render3D();
    ~Render3D();

    virtual void render(const glm::mat4& vMat, const glm::mat4& pMat) = 0;

protected:

    // program handle
    uint32_t _program = 0;
    // vertex array object 
    uint32_t _vao = 0;

    // vertex data
    vector<float> _vertices;
    vector<float> _colors;
    vector<float> _normals;

    uint32_t _vertexCount = 0;

    // model matrix - transforms from model to world space
    glm::mat4 _modelMat;
};

