#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/common.hpp>
#include <glm/matrix.hpp>

#include <math.h>

#include "ProgramLoader.h"
#include "Torus.h"

#define M_PI 3.14159265358979323846f

Torus::Torus(float r, float R, int nr, int nR)
    :_r(r), _R(R), _nr(nr), _nR(nR),
    Render3D()
{
    // load program 
    vector<string> shaderFiles = { "torus.vert", "torus.frag" };
    _program = ProgramLoader::load(shaderFiles);

    // create geometry 
    _createGeometry();
}

Torus::~Torus()
{

}

void Torus::render(const glm::mat4& vMat, const glm::mat4& pMat)
{
    glUseProgram(_program);

    // set matrices
    GLint vMatLoc = glGetUniformLocation(_program, "vMat");
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, &vMat[0][0]);

    GLint pMatLoc = glGetUniformLocation(_program, "pMat");
    glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, &pMat[0][0]);


    glBindVertexArray(_vao);

    glPointSize(4.0);
    // draw 
    glDrawArrays(GL_POINTS, 0, _vertexCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Torus::_createGeometry()
{
    glCreateVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    _createTorus();
    _vertexCount = _vertices.size() / 3;

    GLuint buffer;
    // Get create two buffers
    glCreateBuffers(1, &buffer);
    // Initialize the first buffer
    glNamedBufferStorage(buffer, sizeof(float) * _vertices.size(), _vertices.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 0, buffer, 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 0, 0);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 0);


    glBindVertexArray(0);
}

void Torus::_createTorus()
{
    _vertices.clear();

    // A torus is given by the paramteric equations:
    // x = (R + r cos(v))cos(u)
    // y = (R + r cos(v))sin(u)
    // z = r sin(v)
    // u, v => [0, 2pi]

    float u = 0.0f;
    float du = 2 * M_PI / _nR;
    float v = 0.0f;
    float dv = 2 * M_PI / _nr;

    for (size_t i = 0; i < _nR; i++) {

        for (size_t j = 0; j < _nr; j++) {

            float x = (_R + _r * cos(v)) * cos(u);
            float y = (_R + _r * cos(v)) * sin(u);
            float z = _r * sin(v);

            _vertices.push_back(x);
            _vertices.push_back(y);
            _vertices.push_back(z);

            // incr angle
            v += dv;
        }

        // incr angle 
        u += du;
    }

}
