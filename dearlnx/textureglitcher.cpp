
// OPENCV
#include <highgui.h>

// INTERNAL
#include "textureglitcher.hpp"
#include "texture.h"

TextureGlitcher::TextureGlitcher( int width, int height )
:_width(width), _height(height)
{
	init();
}

TextureGlitcher::~TextureGlitcher()
{
	destroy();
}

void TextureGlitcher::glitch( Texture2D& tex )
{
    glBindTexture( GL_TEXTURE_2D, tex.id() );
    glGetTexImage( GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, _ramBuffer->imageData );
    //cvFlip( _ramBuffer, _ramBuffer, 0 );
		cvCvtColor( _ramBuffer, _threshBuffer, CV_RGB2GRAY );
		cvThreshold( _threshBuffer, _threshBuffer, 25, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU );
		QuadTree q( cvRect( 0, 0, _width, _height ) );
		q.subdivide( 4 );
		q.pruneTree( _threshBuffer, 0.05 );
		//q.mergeTree( _threshBuffer, 0.05 );
		int count = 1;
		while( count != 0 )
		{
			count = q.refit();
		}
		//q.printShit( _ramBuffer );
		std::vector< CvRect > regions = q.retrieveRegions( _threshBuffer );
		cout << "popo:" << regions.size() << endl;
		for( int k = 0; k < regions.size(); ++k )
		{
			CvRect bbox = regions[ k ];
			//cvRectangle( polImg, cvPoint( bbox.x, bbox.y ), cvPoint( bbox.x + bbox.width, bbox.y + bbox.height ), CV_RGB(0,255,255), 1 );
				
			TurboGlitcher g( bbox.width, bbox.height );
			
			IplImage* tn = cvCreateImage( cvSize( bbox.width, bbox.height ), 8, 3 );
			cvSetImageROI( _ramBuffer, bbox );
			cvCopy( _ramBuffer, tn );
			
			if( !(bbox.width < 10 || bbox.height < 10) )
			{
				g.glitch( tn, tn );
				cvCopy( tn, _ramBuffer );
			}
			cvResetImageROI( _ramBuffer );
			cvReleaseImage( &tn );
		}

		//cvSaveImage("polbak.png", _ramBuffer );
		
		// Copy back to texture !
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, _ramBuffer->imageData );
		
}

void TextureGlitcher::resize( int width, int height )
{
	destroy();
	_width = width;
	_height = height;
	init();
}

void TextureGlitcher::init()
{
	_ramBuffer = cvCreateImage( cvSize( _width, _height ), 8, 3 );
	_threshBuffer = cvCreateImage( cvGetSize( _ramBuffer ), 8, 1 );
}

void TextureGlitcher::destroy()
{
	cvReleaseImage( &_ramBuffer );
	cvReleaseImage( &_threshBuffer );
}

