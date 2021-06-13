#include "TextureLoader.h"

#include <stb_image.h>

#include <iostream>

namespace nocg {

// loads texture and returns texture id
unsigned int loadTexture(const string& fileName)
{
    // load image 
    int width, height;
    int nC;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nC, 0);

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
    else {
        std::cout << "Error loading texture from " + fileName << std::endl;
    }

    // free resources
    stbi_image_free(data);

    // return texture id
    return texID;
}

} // namespace nocg