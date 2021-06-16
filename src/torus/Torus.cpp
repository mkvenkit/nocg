#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/common.hpp>
#include <glm/matrix.hpp>

#include <math.h>
#include <iostream>

#include "Utils.h"
#include "Torus.h"

#define M_PI 3.14159265358979323846f

using namespace nocg;

Torus::Torus(float r, float R, int nr, int nR)
    :_r(r), _R(R), _nr(nr), _nR(nR),
    Render3D()
{
    // load program 
    vector<string> shaderFiles = { "torus_p.vert", "torus_p.frag" };
    _program = loadShaders(shaderFiles);

    // create geometry 
    _createGeometry();

}

Torus::~Torus()
{

}

void Torus::render(const glm::mat4& vMat, const glm::mat4& pMat)
{
    glUseProgram(_program);

    //  rim light 
    GLint rlLoc = glGetUniformLocation(_program, "enableRimLight");
    glUniform1f(rlLoc, _rlEnabled ? 1 : 0);


    // set model matrix
    GLint mMatLoc = glGetUniformLocation(_program, "mMat");
    glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, &_modelMat[0][0]);


    // set view matrix
    GLint vMatLoc = glGetUniformLocation(_program, "vMat");
    glUniformMatrix4fv(vMatLoc, 1, GL_FALSE, &vMat[0][0]);

    // set projection matrix 
    GLint pMatLoc = glGetUniformLocation(_program, "pMat");
    glUniformMatrix4fv(pMatLoc, 1, GL_FALSE, &pMat[0][0]);

    // texture settings
    if (_displayMode == eTD_Texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textureID);
        GLint samplerLoc = glGetUniformLocation(_program, "sampler");
        glUniform1i(samplerLoc, 0);
    }

    glBindVertexArray(_vao);

    glPointSize(4.0);
    // draw 
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Torus::setDisplayMode(TorusDisplayMode mode)
{
    // reload if needed 
    _reloadShaders(mode);
    // set current mode
    _displayMode = mode;

    // load texture 
    string strTexFileName;
    if (mode == eTD_Texture) {
        strTexFileName = "tex1.jpg";
    }
    else if (mode == eTD_BumpMapping) {
        strTexFileName = "bm.png";
    }
    _textureID = loadTexture(strTexFileName);
}

void Torus::_createGeometry()
{
    glCreateVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    _createTorus();
    _vertexCount = _vertices.size() / 3;

    GLuint buffer[4];


    // create buffers
    glCreateBuffers(4, buffer);

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

    // tex coords 

    // Initialize the first buffer
    glNamedBufferStorage(buffer[2], sizeof(float) * _texCoords.size(), _texCoords.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 2, buffer[2], 0, 2 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 2, 2, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 2, 2);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 2);

    // tangents 

    // Initialize the first buffer
    glNamedBufferStorage(buffer[3], sizeof(float) * _tangents.size(), _tangents.data(), 0);
    // Bind it to the vertex array - offset zero, stride = sizeof(vec3)
    glVertexArrayVertexBuffer(_vao, 3, buffer[3], 0, 3 * sizeof(float));
    // Tell OpenGL what the format of the attribute is
    glVertexArrayAttribFormat(_vao, 3, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL which vertex buffer binding to use for this attribute
    glVertexArrayAttribBinding(_vao, 3, 3);

    // Enable the attribute
    glEnableVertexArrayAttrib(_vao, 3);


    glBindVertexArray(0);

    // clear memory allocated for vertex data 
    clear();
}

void Torus::_createTorus()
{
    // clear memory allocated for vertex data 
    clear();
    _tangents.clear();

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

                // compute texture coords
                float tx = u / (2 * M_PI);
                float ty = v / (2 * M_PI);

                // add tex coords
                _texCoords.push_back(tx);
                _texCoords.push_back(ty);

                // add tangent vector
                // T = d(S)/du 
                // S(u) is the circle at constant v
                glm::vec3 tg(  -(_R + _r * cos(v)) * sin(uu),
                                (_R + _r * cos(v)) * cos(uu),
                                0.0f
                            );
                tg = glm::normalize(tg);
                _tangents.push_back(tg.x);
                _tangents.push_back(tg.y);
                _tangents.push_back(tg.z);

                //std::cout << tg.x << "," << tg.y << "," << tg.z << std::endl;
            }

            // incr angle
            v += dv;

        }
    }

}

// reload shaders as needed
void Torus::_reloadShaders(TorusDisplayMode mode)
{
    // don't load unnecessarily
    if (_displayMode == mode)
        return;
    
    vector<string> shaderFiles;

    // set shader files
    switch (mode)
    {
    case eTD_Gouraud:
        shaderFiles = { "torus.vert", "torus.frag" };
        break;
    
    case eTD_Phong:
        shaderFiles = { "torus_p.vert", "torus_p.frag" };
        break;

    case eTD_Texture:
        shaderFiles = { "torus_t.vert", "torus_t.frag" };
        break;

    case eTD_ProcTexture:
        shaderFiles = { "torus_tp.vert", "torus_tp.frag" };
        break;

    case eTD_BumpMapping:
        shaderFiles = { "torus_bm.vert", "torus_bm.frag" };
        break;

    default:
        break;
    }

    // load shaders
    if (!shaderFiles.empty()) {
        _program = loadShaders(shaderFiles);
    }
}
