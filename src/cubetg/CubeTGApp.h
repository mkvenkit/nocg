#pragma once

#include <string>
#include <memory>

using std::string;

#include "RenderApp.h"

class PlaneTG;
class Axis3D;

class CubeTGApp : public nocg::RenderApp
{

public:

    CubeTGApp(int width, int height, const string& appName);
    virtual ~CubeTGApp();

    // override from base class
    void render();

    // override from base class
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // override as needed
    void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);

    // override from base class
    void step();

private:

    std::unique_ptr<PlaneTG> _plane;
    std::unique_ptr<Axis3D> _axis;

    bool _showAxis = true;
    bool _showPlane = false;
};
