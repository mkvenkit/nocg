#pragma once

#include "Render3D.h"

using glm::vec2;


class Plane :
    public nocg::Render3D
{

public:

    Plane(const vec2& dims);
    ~Plane();

    // Inherited via Render3D
    void render(const glm::mat4& vMat, const glm::mat4& pMat);

private:

    void _createGeometry();

    void _createPlane();

    // dimensions - x-length, y-length
    glm::vec2 _dims;
};

