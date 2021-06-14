#pragma once

#include <string>
#include <vector>

#include "Render3D.h"

using std::string;
using std::vector;

typedef enum _TorusDisplayMode {

    eTD_Gouraud = 0,
    eTD_Phong,
    eTD_texture

} TorusDisplayMode;

class Torus : public nocg::Render3D
{

public:

    // constructor, etc.
    Torus(float r, float R, int nr, int nR);
    ~Torus();

    // public methods 
    void render(const glm::mat4& vMat, const glm::mat4& pMat);

    // set display mode 
    void setDisplayMode(TorusDisplayMode mode);

private:

    // METHODS 
    void _createGeometry();

    void _createTorus();

    void _reloadShaders(TorusDisplayMode mode);


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

    TorusDisplayMode _displayMode = eTD_Gouraud;
};

