#pragma once

#include <string>
#include <memory>

using std::string;

#include "RenderApp.h"

class Torus;
class Axis3D;
class Camera;

class TorusApp :
    public RenderApp
{

public:

    TorusApp(int width, int height, const string& appName);
    ~TorusApp();

    void render();

private:

    std::unique_ptr<Torus> _torus;

    std::unique_ptr<Axis3D> _axis;
    std::unique_ptr<Camera> _camera;

};

