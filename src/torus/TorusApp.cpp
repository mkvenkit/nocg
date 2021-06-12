#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Torus.h"
#include "Axis3D.h"
#include "Camera.h"
#include "TorusApp.h"

#define M_PI 3.14159265358979323846f

TorusApp::TorusApp(int width, int height, const string& appName)
    :RenderApp(width, height, appName)
{
    // set time step interval for animation
    setTStep(0.1);

    _torus = std::make_unique<Torus>(0.5, 2, 64, 64);
    _axis = std::make_unique<Axis3D>(10.0);
}

TorusApp::~TorusApp()
{

}

void TorusApp::render()
{
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //_torus->render();

    // projection transform 
    float fov = 35 * M_PI / 180.0f;
    glm::mat4 pMat = glm::perspective(fov, _aspect, 1.0f, 100.0f);

    // view transform
    glm::vec3 eye(8, 8, 8);
    glm::vec3 center(0, 0, 0);
    glm::vec3 up(0, 0, 1);
    glm::mat4 vMat = glm::lookAt(eye, center, up);

    // render torus 
    _torus->render(vMat, pMat);

    // render axis
    if (_showAxis) {
        _axis->render(vMat, pMat);
    }

}

void TorusApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
 
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        _torus->togglePhongShading();
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        _torus->toggleRimLight();
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        // toggle axis display
        _showAxis = !_showAxis;
    }

    // call base class method
    RenderApp::keyCallback(window, key, scancode, action, mods);
}

void TorusApp::step()
{
    // compute angle
    static float theta = 0.0;
    theta = theta + 2.0 * M_PI / 180.0f;

    // set model transform
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0.0f, 1.0f, 0.0f));
    _torus->setModelMatrix(trans * rot);

}
