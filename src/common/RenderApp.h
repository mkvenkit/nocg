#pragma once

#include <string>
using std::string;

struct GLFWwindow;

class RenderApp
{

public:

    RenderApp(int width, int height, const string& appName);
    virtual ~RenderApp();

    // main loop
    void run();

    // override as needed 
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // override this in derived class
    virtual void render();

    // override as needed
    virtual void step() {}

protected:

    int _width = 0;
    int _height = 0;
    float _aspect = 1.0;

    string _appName;

    // set time step interval in seconds
    void setTStep(float tsInterval)  {
        _tsInterval = tsInterval;
    }

private:

    //
    // Private methods
    //
    
    // hide
    RenderApp() {}

    void _glfwInit();
    void _glInit();

    //
    // Private data
    //

    // time between steps
    float _tsInterval = 0.25f;
    
    // time in seconds since start
    float _time = 0.0f;

    // time in seconds for last step()
    float _lastStep = 0.0;

    GLFWwindow* _window = 0;

};

