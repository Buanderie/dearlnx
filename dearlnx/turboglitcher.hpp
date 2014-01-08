#ifndef __TURBOGLITCHER_HPP__
#define __TURBOGLITCHER_HPP__

// SYSTEM
#include <cstdlib>

// STL
#include <algorithm>
#include <iostream>

// OPENCV
#include <opencv/cv.h>

// jpeg
#include <turbojpeg.h>

using namespace std;

class TurboGlitcher
{
public:
	TurboGlitcher( unsigned int width, unsigned int height )
	:_width(width), _height(height)
	{
		init();
	}

	virtual ~TurboGlitcher(){
		deinit();
	}

	void glitch( IplImage* img, IplImage* dst )
	{
		cvCopy( img, dst );

		// preps
		unsigned char* input_img = (unsigned char*)img->imageData;
		long unsigned int _jpegSize = _width * _height * 3;

		// encoding
		tjCompress2(_jpegCompressor, input_img, _width, 0, _height, TJPF_RGB,
          &_jpegBuffer, &_jpegSize, TJSAMP_444, 80,
          TJFLAG_FASTDCT);

		// Random glitching
        int headerSize = getJpegHeaderSize( (const unsigned char*)_jpegBuffer, _width*_height*3 );
        glitchJpegBytes( (unsigned char*)_jpegBuffer, _width*_height*3, headerSize, 256.0, 100+rand()%100 );

		// decoding
		_jpegDecompressor = tjInitDecompress();
		int jpegSubsamp;
		int width = _width;
		int height = _height;
		tjDecompressHeader2( _jpegDecompressor, _jpegBuffer, _jpegSize, &width, &height, &jpegSubsamp);
		tjDecompress2( _jpegDecompressor, _jpegBuffer, _jpegSize, _outputBuffer, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);
		memcpy( dst->imageData, _outputBuffer, _width * _height * 3 );

	}

private:
	unsigned char * _jpegBuffer;
	unsigned char * _outputBuffer;
	unsigned		_width;
	unsigned		_height;
	tjhandle		_jpegCompressor; 
	tjhandle		_jpegDecompressor;

	void init()
	{
		_jpegCompressor = tjInitCompress();

		// Allocate (w*h*3)
		_jpegBuffer = new unsigned char[ _width * _height * 3 ];

		// Allocate (w*h*3)
		_outputBuffer = new unsigned char[ _width * _height * 3 ];
	}

	void deinit()
	{
		tjDestroy(_jpegCompressor);
		tjDestroy(_jpegDecompressor);
		delete[] _jpegBuffer;
		delete[] _outputBuffer;
	}

	int getJpegHeaderSize( const unsigned char* data, int datalen )
	{
		int result = 417;
		for ( int i = 0; i < datalen; i++ )
		{
    		if ( data[i] == 0xFF && data[i + 1] == 0xDA )
			{
        		result = i + 2;
				break;
			}
		}
		return result;
	}

	void glitchJpegBytes( unsigned char* data, int datalen, int headerLength, double glitchAmount, int niter )
{
	for( int k = 0; k < niter; ++k )
	{
		int max_index = datalen - headerLength - 4;
		int px_min = headerLength + 4;
		int px_max = max_index;
		int delta = px_max - px_min;
		int px_i = px_min + (double)delta * ((double)rand()/(double)RAND_MAX);
			
		int idx = headerLength + px_i;
		
		std::swap( data[ idx ], data[ idx + 1 ] );
	}
}

void fast_unpack(char* rgba, const char* rgb, const int width, const int height ) 
{
		for(int i = width*height; --i; rgba+=4, rgb+=3) {
		    *(uint32_t*)(void*)rgba = *(const uint32_t*)(const void*)rgb;
		}
}

protected:

};

#endif
