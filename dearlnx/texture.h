#pragma once
#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <GL/glew.h>

#include <SOIL/SOIL.h>

template< GLuint T >
class Texture {
public:
    static const GLuint TextureType = T;

    Texture()
    {
        glGenTextures(1, &_id);
    }
    
    Texture( const std::string& filePath )
    {
    	_id = SOIL_load_OGL_texture( filePath.c_str(), 4, 0, SOIL_FLAG_TEXTURE_REPEATS );
    }

    GLuint id() const
    {
        return _id;
    }

    void bind() const
    {
        glBindTexture(TextureType, _id);
    }

		void save( const std::string& filePath )
		{
			
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

    void downloadData( void* ptr, int w, int h )
    {
      glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, ptr);
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
