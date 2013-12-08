#pragma once
#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <GL/glew.h>

template< GLuint T >
class Texture {
public:
    static const GLuint TextureType = T;

    Texture()
    {
        glGenTextures(1, &_id);
    }

    GLuint id() const
    {
        return _id;
    }

    void bind() const
    {
        glBindTexture(TextureType, _id);
    }

    void regenerate()
    {
        glDeleteTextures(1,&_id);
        glGenTextures(1, &_id);
    }

    static void unbind()
    {
        glBindTexture(TextureType, 0);
    }

    ~Texture()
    {
        glDeleteTextures(1, &_id);
    }
    
private:
    GLuint _id;
};

typedef Texture<GL_TEXTURE_2D> Texture2D;
typedef Texture<GL_TEXTURE_1D> Texture1D;
typedef Texture<GL_TEXTURE_3D> Texture3D;


#endif