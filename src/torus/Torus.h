#pragma once

#include <string>
#include <vector>

#include "Render3D.h"

using std::string;
using std::vector;

class Torus : public nocg::Render3D
{

public:

    // constructor, etc.
    Torus(float r, float R, int nr, int nR);
    ~Torus();

    // public methods 
    void render(const glm::mat4& vMat, const glm::mat4& pMat);

    // toggle lighting method
    void togglePhongShading();

    // toggle rim lighting 
    void toggleRimLight();


private:

    // METHODS 
    void _createGeometry();

    void _createTorus();


    // DATA 

    // torus parameters
    float _r = 0.5f;
    float _R = 1.0f;
    int _nr = 16;
    int _nR = 16;

    // using phong shading
    bool _usingPhong = true;

    // rim lighting 
    bool _rlEnabled = false;
};

