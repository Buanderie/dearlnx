
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
#include "camera.hpp"

// LOL
#include <GLFW/glfw3.h>
#define GLFW_STATIC

using namespace std;

void usage( int argc, char** argv )
{

}

FrameBuffer* fbo;
Camera* cam;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
			//fbo->resize( width, height );
      glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        cam->rotateYaw(1.0f);
        
        if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        cam->rotateYaw(-1.0f);
        
        if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
        cam->rotatePitch(0.1f);
        
        if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        cam->rotatePitch(-1.0f);
        
        if (key == GLFW_KEY_Z && action == GLFW_REPEAT)
        cam->forward(-0.1f);
        
        if (key == GLFW_KEY_Q && action == GLFW_REPEAT)
        cam->forward(0.1f);
        
        if (key == GLFW_KEY_S && action == GLFW_REPEAT)
        cam->strafe(-0.1f);
        
        if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        cam->strafe(0.1f);
}

int main( int argc, char** argv )
{
	/***************
	bool quitApp = false;
	bool pauseApp = false;
	
	IplImage* frame;
	IplImage* polImg;
	
	while(1)
	{
		if( !pauseApp )
			frame = cvLoadImage( argv[1] );

		if( frame == NULL || quitApp )
			break;

		unsigned int twidth = 800;
		unsigned int theight = 600;

		polImg = cvCreateImage( cvSize( twidth, theight ), 8, 3 );
		cvResize( frame, polImg );

		IplImage* threshImg = cvCreateImage( cvGetSize( polImg ), 8, 1 );
		
		//sf::Clock clock;
		cvCvtColor( polImg, threshImg, CV_RGB2GRAY );
		cvThreshold( threshImg, threshImg, 25, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU );

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
		cvShowImage( "bak", threshImg );

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
		//double elapsed = clock.getElapsedTime().asSeconds();
		//cout << "elapsed=" << elapsed << " - fps=" << 1.0 / elapsed << endl;

		//cvReleaseBlobs(blobs);
		cvReleaseImage( &polImg );
		cvReleaseImage( &frame );
		cvReleaseImage( &threshImg );
	}
	
	*/

	cam = new Camera();

	
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
	 glfwSetKeyCallback(window, key_callback);
	//fbo = new FrameBuffer( 1, 640, 480 );
	Shader basic_shader( "resources/shaders/basic_vs.txt", "resources/shaders/basic_fs.txt" );

	int w, h;
	double curx, cury, lastcurx, lastcury;
  glfwGetWindowSize( window, &w, &h );

	Texture2D ttt( "resources/ananas.jpg" );
	
	QuadRenderer quad;
	double frameTime = 0.0f;
	
    while (!glfwWindowShouldClose(window))
    {
    
    	//cam.forward(frameTime * 0.05f);
    	
    	
	glm::vec3 pos = cam->getPosition();
	//cout << pos[0] << " - " << pos[1] << " - " << pos[2] << endl;
	
		float deltax = lastcurx - curx;
		
		glfwGetCursorPos( window, &curx, &cury );
		//cam.rotateYaw( 50 * frameTime);
		//cam.rotatePitch( 5.0 * frameTime);
		double then = glfwGetTime();	
		/*
		fbo->bind();
    basic_shader.setUniform1f( "polbak", then );
    basic_shader.setUniform1f( "resx", w );
    basic_shader.setUniform1f( "resy", h );
    basic_shader.setTextureSampler( 0, ttt );
		basic_shader.bind();
		quad.draw();
		basic_shader.unbind();
		IplImage* pompom = cvCreateImage( cvSize( w, h ), 8, 4 );
		glReadPixels( 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pompom->imageData );
		cvSaveImage("lolpute.png", pompom );
		cvReleaseImage( &pompom );
		fbo->unbind();
		*/
		
    glfwGetWindowSize( window, &w, &h );
    basic_shader.setUniform1f( "polbak", then );
    basic_shader.setUniform1f( "resx", w );
    basic_shader.setUniform1f( "resy", h );
    basic_shader.setUniformVec3( "campos", cam->getPosition() );
    basic_shader.setUniformMat4( "camrot", cam->getOrientationMatrix() );
    //basic_shader.setTextureSampler( 0, fbo->getTexture(0) );
    basic_shader.bind();
    quad.draw();
    basic_shader.unbind();
    
        glfwSwapBuffers(window);
       
        glfwPollEvents();

		double now = glfwGetTime();
		double elapsed = now - then;
		frameTime = elapsed;
		//cout << "t=" << elapsed << " - fps=" << 1.0/elapsed << endl;
    }

    glfwTerminate();
	
    return 0;
}
