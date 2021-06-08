#pragma once

#include <string>
#include <vector>

#include "Render3D.h"

using std::string;
using std::vector;

class Torus : public Render3D
{

public:

    // constructor, etc.
    Torus(float r, float R, int nr, int nR);
    ~Torus();

    // public methods 
    void render(const glm::mat4& vMat, const glm::mat4& pMat);

private:

    // METHODS 
    void _createGeometry();


    // DATA 

    // torus parameters
    float _r = 0.5f;
    float _R = 1.0f;
    int _nr = 16;
    int _nR = 16;

    // 


};

