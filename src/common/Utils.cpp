/*


*/

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/common.hpp>

#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;

#include <filesystem>
namespace fs = std::filesystem;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


namespace nocg {

static bool getenv(const string& var, string& value)
{
    char* envvar;
    size_t requiredSize;

    getenv_s(&requiredSize, NULL, 0, var.c_str());
    if (requiredSize == 0)
    {
        cout << var << "not set!" << endl;
        return false;
    }

    envvar = new char[requiredSize];
    if (!envvar)
    {
        printf("Failed to allocate memory!\n");
        return false;
    }

    // get the value of the env variable
    getenv_s(&requiredSize, envvar, requiredSize, var.c_str());

    value = string(envvar);

    delete[] envvar;

    return true;
}

static GLuint loadShader(GLenum shaderType, const string& fileName)
{
    GLuint shader = 0;

    std::stringstream buffer;
    try
    {
        std::ifstream file(fileName);
        buffer << file.rdbuf();
    }
    catch (const std::exception& ex)
    {
        cout << ex.what() << endl;
        return 0;
    }

    string strShader = buffer.str();
    const char* c_str = strShader.c_str();
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        std::string strLog(infoLog.begin(), infoLog.end());
        cout << strLog << endl;

        // We don't need the shader anymore.
        glDeleteShader(shader);
    }

    return shader;
}

unsigned int loadShaders(vector<string> shaderFiles)
{
    // get shader dir
    string shader_dir;
    bool success = getenv("NOCG_SHADER_DIR", shader_dir);
    if (success) {
        cout << "Looking for shaders in " + string(shader_dir) << "..." << endl;
    }
    else {
        cout << "Error loading shaders!" << endl;
        return 0;
    }

    GLuint program = glCreateProgram();

    for (auto& fileName : shaderFiles) {

        fs::path p(fileName);
        const auto& fext = p.extension();
        fileName = shader_dir + "/" + fileName;

        if (fext == ".vert") {
            GLuint shader = loadShader(GL_VERTEX_SHADER, fileName);
            glAttachShader(program, shader);
        }
        else if (fext == ".frag") {
            GLuint shader = loadShader(GL_FRAGMENT_SHADER, fileName);
            glAttachShader(program, shader);
        }
    }

    glLinkProgram(program);

    return program;
}


// loads texture and returns texture id
unsigned int loadTexture(const string& fileName)
{
    // get texture dir
    // if NOCG_TEXTURE_DIR is not set, assume textures/ 
    // relative to where exe is run from 

    string filePath;
    string tex_dir;
    bool success = getenv("NOCG_TEXTURE_DIR", tex_dir);
    if (success) {
        filePath = tex_dir + "/" + fileName;
    }
    else {
        filePath = "./textures/" + fileName;
    }

    cout << "Looking for texture in " + filePath << "..." << endl;

    // load image 
    int width, height;
    int nC;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nC, 0);

    unsigned int texID = 0;

    if (data) {
        // set texture format 
        GLenum format;
        switch (nC)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
        }

        // setup texture 
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    }

    return texID;
}

} // namespace nocg