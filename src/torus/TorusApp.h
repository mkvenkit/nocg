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

    // override from base class
    void render();

    // override from base class
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


private:

    std::unique_ptr<Torus> _torus;

    std::unique_ptr<Axis3D> _axis;
    std::unique_ptr<Camera> _camera;

};

