#pragma once

#include <string>
using std::string;

struct GLFWwindow;

class RenderApp
{

public:

    RenderApp(int width, int height, const string& appName);
    virtual ~RenderApp();

    // override as needed 
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void run();

    // override this in derived class
    virtual void render();

protected:

    int _width = 0;
    int _height = 0;
    float _aspect = 1.0;

    string _appName;


private:

    // hide
    RenderApp() {}

    void _glfwInit();
    void _glInit();

    GLFWwindow* _window = 0;

};

