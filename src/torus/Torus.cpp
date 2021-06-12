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
    vector<string> shaderFiles = { "torus_p.vert", "torus_p.frag" };
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
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Torus::_createGeometry()
{
    glCreateVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    _createTorus();
    _vertexCount = _vertices.size() / 3;

    GLuint buffer[2];


    // Get create two buffers
    glCreateBuffers(2, buffer);

    // vertices 
    
    // Initialize the first buffer
    glNamedBufferStorage(buffer[0], sizeof(float) * _vertices.size(), _vertices.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 0, buffer[0], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 0, 0);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 0);

    // normals 

    // Initialize the first buffer
    glNamedBufferStorage(buffer[1], sizeof(float) * _normals.size(), _normals.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 1, buffer[1], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 1, 1);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 1);


    glBindVertexArray(0);
}

void Torus::_createTorus()
{
    // clear old
    _vertices.clear();
    _normals.clear();

    // A torus is given by the paramteric equations:
    // x = (R + r cos(v))cos(u)
    // y = (R + r cos(v))sin(u)
    // z = r sin(v)
    // u, v => [0, 2pi]


    float du = 2 * M_PI / _nR;
    float dv = 2 * M_PI / _nr;

    for (size_t i = 0; i < _nR; i++) {
        
        float u = i * du;

        for (size_t j = 0; j <= _nr; j++) {

            float v = (j % _nr) * dv;

            for (size_t k = 0; k < 2; k++)
            {
                float uu = u + k * du;
                // compute vertex
                float x = (_R + _r * cos(v)) * cos(uu);
                float y = (_R + _r * cos(v)) * sin(uu);
                float z = _r * sin(v);

                // add vertex 
                _vertices.push_back(x);
                _vertices.push_back(y);
                _vertices.push_back(z);

                // compute normal 
                float nx = cos(v) * cos(uu);
                float ny = cos(v) * sin(uu);
                float nz = sin(v);

                // add normal 
                _normals.push_back(nx);
                _normals.push_back(ny);
                _normals.push_back(nz);

                // incr angle
                v += dv;
            }
        }
    }

}
