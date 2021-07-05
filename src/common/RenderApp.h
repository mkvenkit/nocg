#pragma once

#include <string>
#include <glm/common.hpp>
#include <glm/matrix.hpp>

using std::string;

struct GLFWwindow;

namespace nocg {

class RenderApp
{

public:

    RenderApp(int width, int height, const string& appName);
    virtual ~RenderApp();

    // main loop
    void run();

    // override as needed 
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // override as needed
    virtual void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);

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

    // projecttion matrix
    glm::mat4 _pMat;
    // vuiew matrix 
    glm::mat4 _vMat;

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

} // namespace nocg