#pragma once

#include "Render3D.h"

using glm::vec2;


class PlaneTG :
    public nocg::Render3D
{

public:

    PlaneTG(const vec2& dims);
    ~PlaneTG();

    // Inherited via Render3D
    void render(const glm::mat4& vMat, const glm::mat4& pMat);

private:

    void _createGeometry();

    void _createPlaneTG();

    // dimensions - x-length, y-length
    glm::vec2 _dims;
};

