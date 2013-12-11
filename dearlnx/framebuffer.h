#ifndef __FRAMEBUFFER_HPP__
#define __FRAMEBUFFER_HPP__

// STL
#include <vector>

// GLEW
#include <GL/glew.h>

// INTERNAL
#include "texture.h"

class FrameBuffer
{
public:
	FrameBuffer( int nbTextures, int width, int height );
	virtual ~FrameBuffer(){}

	GLuint id(){ return _id; }
	
	void bind() const
  {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
  }

	static void unbind()
  {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

	Texture2D& getTexture( int id )
	{
		return *(_textures[id] );
	}

  void resize( int width, int height );

private:
	// Our FBO id
	GLuint _id;

	// A collection of textures
	std::vector< Texture2D* > _textures;
	int _nbTex;

	// Private methods
	void init( int nbTextures, int width, int height );
  void destroy();

protected:

};

#endif
