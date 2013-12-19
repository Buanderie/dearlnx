
// C
#include <ctype.h>
#include <cstdlib>

// STL
#include <iostream>

// OpenCV ?
#include <opencv/cv.h>
#include <opencv/highgui.h>

// Internal
#include "turboglitcher.hpp"
#include "quadtree.hpp"
#include "texture.h"
#include "framebuffer.h"
#include "quadrenderer.h"
#include "shader.hpp"

// LOL
#include <GLFW/glfw3.h>
#define GLFW_STATIC

using namespace std;

void usage( int argc, char** argv )
{

}

FrameBuffer* fbo;
	
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
			fbo->resize( width, height );
      glViewport(0, 0, width, height);
}

int main( int argc, char** argv )
{
	/*
	bool quitApp = false;
	bool pauseApp = false;
	
	IplImage* frame;
	IplImage* polImg;
	
	while(1)
	{
		if( !pauseApp )
			frame = cvLoadImage( "C:\\dede.jpg" );

		if( frame == NULL || quitApp )
			break;

		unsigned int twidth = 800;
		unsigned int theight = 600;

		polImg = cvCreateImage( cvSize( twidth, theight ), 8, 3 );
		cvResize( frame, polImg );

		IplImage* threshImg = cvCreateImage( cvGetSize( polImg ), 8, 1 );
		
		sf::Clock clock;
		cvCvtColor( polImg, threshImg, CV_RGB2GRAY );
		cvThreshold( threshImg, threshImg, 25, 255, CV_THRESH_BINARY | CV_THRESH_OTSU );

		QuadTree q( cvRect( 0, 0, twidth, theight ) );
		q.subdivide( 4 );
		q.pruneTree( threshImg, 0.10 );
		//q.mergeTree( threshImg, 0.05 );
		int count = 1;
		while( count != 0 )
		{
			count = q.refit();
		}

		//q.printShit( polImg );

		std::vector< CvRect > regions = q.retrieveRegions( threshImg );
		cout << "popo:" << regions.size() << endl;
		for( int k = 0; k < regions.size(); ++k )
		{
			CvRect bbox = regions[ k ];
			//cvRectangle( polImg, cvPoint( bbox.x, bbox.y ), cvPoint( bbox.x + bbox.width, bbox.y + bbox.height ), CV_RGB(0,255,255), 1 );
				
			TurboGlitcher g( bbox.width, bbox.height );
			
			IplImage* tn = cvCreateImage( cvSize( bbox.width, bbox.height ), 8, 3 );
			cvSetImageROI( polImg, bbox );
			cvCopy( polImg, tn );
			
			if( !(bbox.width < 10 || bbox.height < 10) )
			{
				g.glitch( tn, tn );
				cvCopy( tn, polImg );
			}

			cvResetImageROI( polImg );
			cvReleaseImage( &tn );
		}
		
		cvShowImage( "dearlnx", polImg );
		//cvShowImage( "bak", threshImg );

		char c = cvWaitKey( 5 );
		switch( c )
		{
			case 'q':
				quitApp = true;
				break;
			case ' ':
				pauseApp = !pauseApp;
				break;
				
			default:
				break;
		}
		double elapsed = clock.getElapsedTime().asSeconds();
		cout << "elapsed=" << elapsed << " - fps=" << 1.0 / elapsed << endl;

		//cvReleaseBlobs(blobs);
		cvReleaseImage( &polImg );
		cvReleaseImage( &frame );
		cvReleaseImage( &threshImg );
	}
	*/
	

	GLFWwindow* window;	


    if (!glfwInit())
        return -1;
	
		GLFWmonitor* monitor = glfwGetPrimaryMonitor	();
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

  glfwMakeContextCurrent(window);

	glewExperimental=TRUE;
	GLenum err=glewInit();
	if(err!=GLEW_OK)
	{
    //Problem: glewInit failed, something is seriously wrong.
		cout<<"glewInit failed, aborting."<<endl;
		string dump;
		cin >> dump;
		return -1;
	}

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	fbo = new FrameBuffer( 1, 640, 480 );
	Shader basic_shader( "resources/shaders/basic_vs.txt", "resources/shaders/basic_fs.txt" );

	int w, h;
    glfwGetWindowSize( window, &w, &h );

	Texture2D ttt( "resources/ananas.jpg" );
	
	QuadRenderer quad;

    while (!glfwWindowShouldClose(window))
    {
		double then = glfwGetTime();	
		
		fbo->bind();
    basic_shader.setUniform1f( "polbak", then );
    basic_shader.setUniform1f( "resx", w );
    basic_shader.setUniform1f( "resy", h );
    basic_shader.setTextureSampler( 0, ttt );
		basic_shader.bind();
		quad.draw();
		basic_shader.unbind();
		fbo->unbind();

    glfwGetWindowSize( window, &w, &h );
    basic_shader.setUniform1f( "polbak", then );
    basic_shader.setUniform1f( "resx", w );
    basic_shader.setUniform1f( "resy", h );
    basic_shader.setTextureSampler( 0, fbo->getTexture(0) );
    basic_shader.bind();
    quad.draw();
    basic_shader.unbind();
    
        glfwSwapBuffers(window);
       
        glfwPollEvents();

		double now = glfwGetTime();
		double elapsed = now - then;
		cout << "t=" << elapsed << " - fps=" << 1.0/elapsed << endl;
    }

    glfwTerminate();
	
    return 0;
}
