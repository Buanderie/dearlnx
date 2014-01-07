#ifndef __TEXTUREGLITCHER_HPP__
#define __TEXTUREGLITCHER_HPP__

// INTERNAL
#include "turboglitcher.hpp"
#include "quadtree.hpp"
#include "texture.h"

class TextureGlitcher
{
	public:
		TextureGlitcher( int width, int height );
		virtual ~TextureGlitcher();
		
		void glitch( Texture2D& tex );
		void resize( int width, int height );
		
	private:
		int _width;
		int _height;
		IplImage* _ramBuffer;
		IplImage* _threshBuffer;
		
		
		void init();
		void destroy();
		
	protected:
	
};

#endif
