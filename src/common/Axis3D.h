#pragma once

#include "Render3D.h"

class Axis3D :
    public Render3D
{
public:

    Axis3D(float length);
    ~Axis3D();

    // Inherited via Render3D
    void render(const glm::mat4& vMat, const glm::mat4& pMat);

private:

    void _createGeometry();

    // axis length 
    float _length = 1.0f;

};

