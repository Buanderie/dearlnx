
// C
#include <ctype.h>
#include <cstdlib>

// STL
#include <iostream>

// OpenCV ?

#include <opencv/cv.h>
#include <opencv/highgui.h>


// Internal
#include "textureglitcher.hpp"
#include "texture.h"
#include "framebuffer.h"
#include "quadrenderer.h"
#include "shader.hpp"
#include "camera.hpp"
#include "spline.hpp"

// LOL
#include <GLFW/glfw3.h>
#define GLFW_STATIC

using namespace std;

void usage( int argc, char** argv )
{

}

Spline* spline;
FrameBuffer* fbo;
TextureGlitcher* tg;
Camera* cam;
double frameTime;
int glitchDuration;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
			fbo->resize( width, height );
			tg->resize( width, height );
      glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        cam->rotateYaw(1.0f);
        
        if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        cam->rotateYaw(-1.0f);
        
        if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
        cam->rotatePitch(1.0f);
        
        if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        cam->rotatePitch(-1.0f);
        
        if (key == GLFW_KEY_W && action == GLFW_REPEAT)
        cam->forward(1.0f);
        
        if (key == GLFW_KEY_S && action == GLFW_REPEAT)
        cam->forward(-1.0f);
        
        if (key == GLFW_KEY_A && action == GLFW_REPEAT)
        cam->strafe(-1.0f);
        
        if (key == GLFW_KEY_D && action == GLFW_REPEAT)
        cam->strafe(1.0f);
        
        if( key == GLFW_KEY_SPACE && action == GLFW_PRESS )
        {
        	spline->addKeyPoint( frameTime, cam->getPosition() );
        	spline->save( "spline.txt" );
        	
        }
}

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int main( int argc, char** argv )
{

	srand( time(NULL) );
	glitchDuration = 0;
	
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
  
	cam = new Camera();
	
	spline = new Spline();
	spline->load( "spline.txt" );
	
	GLFWwindow* window;	


    if (!glfwInit())
        return -1;
	
	glfwSetErrorCallback(error_callback);
	
		GLFWmonitor* monitor = glfwGetPrimaryMonitor	();
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
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
	fbo = new FrameBuffer( 1, SCREEN_WIDTH, SCREEN_HEIGHT );
	tg = new TextureGlitcher( SCREEN_WIDTH, SCREEN_HEIGHT );
	
	Shader basic_shader( "resources/shaders/basic_vs.txt", "resources/shaders/basic_fs.txt" );
	Shader texdraw_shader( "resources/shaders/basic_vs.txt", "resources/shaders/texdraw_fs.txt" );
	
	int w, h;
	double curx, cury, lastcurx, lastcury;
  glfwGetWindowSize( window, &w, &h );

	//Texture2D ttt( "resources/ananas.jpg" );
	
	QuadRenderer quad;
	

	
    while (!glfwWindowShouldClose(window))
    {
    
    	//cam.forward(frameTime * 0.05f);
    	glfwGetWindowSize( window, &w, &h );
    	double mposX, mposY;
			glfwGetCursorPos(window, &mposX, &mposY);
			glfwSetCursorPos(window, (double)(w / 2), (double)(h / 2));
			double dx = mposX - w / 2;
			double dy = mposY - h / 2;
			cam->rotateYaw( dx * 0.1 );
			cam->rotatePitch( dy * 0.1 );
			
			cout << "dx=" << dx << " - dy=" << dy << endl;
	
		float deltax = lastcurx - curx;
		
		//cam.rotateYaw( 50 * frameTime);
		//cam.rotatePitch( 5.0 * frameTime);
		double then = glfwGetTime();	
		frameTime = then;
		
		//cam->setPosition( glm::vec3( 50.0 * cos( then ), 10.0 + 100.0 * cos(then), 50.0 * -sin( then ) ) );	
		
		//
		cout << "frameTime=" << frameTime << endl;
		glm::vec3 popopo = spline->getPointFromSync( frameTime );
		cam->setPosition( popopo );
		//
			 
		glm::vec3 pos = cam->getPosition();
		cout << pos[0] << " - " << pos[1] << " - " << pos[2] << endl;
			
		cam->update();
		cam->lookAt( glm::vec3(0, 10.0, 0) );
			
    fbo->bind();
    basic_shader.setUniform1f( "polbak", then );
    basic_shader.setUniform1f( "resx", w );
    basic_shader.setUniform1f( "resy", h );
    basic_shader.setUniformVec3( "campos", cam->getPosition() );
    basic_shader.setUniformMat4( "camrot", cam->getOrientationMatrix() );
    basic_shader.setUniformVec3( "camheading", cam->getViewVector() );
    basic_shader.setUniformVec3( "camup", cam->getUpVector() );
    basic_shader.setUniformVec3( "camright", cam->getRightVector() );
    //basic_shader.setTextureSampler( 0, fbo->getTexture(0) );
    basic_shader.bind();
    quad.draw();
    basic_shader.unbind();
    fbo->unbind();
    
    if( glitchDuration == 0 && (long)(frameTime * 100.0f) % rand()%15 == 0 )
    	glitchDuration = 15;
    	
    if( glitchDuration > 0 )
    {
    	tg->glitch( fbo->getTexture(0) );
    	glitchDuration--;
    }
    	
    texdraw_shader.setUniform1f( "resx", w );
    texdraw_shader.setUniform1f( "resy", h );
    texdraw_shader.setTextureSampler( 0, fbo->getTexture(0) );
    texdraw_shader.bind();
    quad.draw();
    texdraw_shader.unbind();
    
    glfwSwapBuffers(window);
    		
    glfwPollEvents();
		    
		double now = glfwGetTime();
		double elapsed = now - then;
		cout << "t=" << elapsed << " - fps=" << 1.0/elapsed << endl;
    }

    glfwTerminate();
	  
    return 0;
}
