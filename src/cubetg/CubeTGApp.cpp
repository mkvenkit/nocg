#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
using std::cout;
using std::endl;

#include "nocg_common.h"
#include "PlaneTG.h"
#include "Axis3D.h"

#include "CubeTGApp.h"

CubeTGApp::CubeTGApp(int width, int height, const string& appName)
    :RenderApp(width, height, appName)
{
    // set time step interval for animation
    setTStep(0.1);

    _axis = std::make_unique<Axis3D>(10.0);
    _plane = std::make_unique<PlaneTG>(glm::vec2(2.f, 2.f));

    //_printHelp();

}

CubeTGApp::~CubeTGApp()
{

}

// override as needed
void CubeTGApp::mouseBtnCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        cout << xpos << "," << ypos << endl;

        glm::vec4 viewport(0.f, 0.f, (float)_width, (float)_height);
        _plane->hitTest(xpos, (double)_height - ypos, _vMat, _pMat, viewport);
    }
}

void CubeTGApp::render()
{
    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //_torus->render();

    // projection transform 
    float fov = 35 * M_PI / 180.0f;
    _pMat = glm::perspective(fov, _aspect, 1.0f, 100.0f);

    // view transform
    glm::vec3 eye(8, 8, 8);
    glm::vec3 center(0, 0, 0);
    glm::vec3 up(0, 0, 1);
    _vMat = glm::lookAt(eye, center, up);

    // render plane
    _plane->render(_vMat, _pMat);
    

    // render axis
    if (_showAxis) {
        _axis->render(_vMat, _pMat);
    }

}

void CubeTGApp::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // call base class method
    RenderApp::keyCallback(window, key, scancode, action, mods);
}

void CubeTGApp::step()
{
    // compute angle
    static float theta = 0.0;
    theta = theta + 2.0 * M_PI / 180.0f;

    // set model transform
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0.0f, 1.0f, 0.0f));
    _plane->setModelMatrix(trans * rot);
}
