#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
  public:
    Camera();
    virtual ~Camera();

		void update();
		glm::mat4 getOrientationMatrix();
		glm::vec3 getPosition();
		glm::vec3 getViewVector();
		glm::vec3 getUpVector();
		glm::vec3 getRightVector();
		
		void setPosition( glm::vec3 pos ){ _pos = pos; }
		
		void forward( float speed );
		void strafe( float speed );
		void rotatePitch( float speed );
		void rotateYaw( float speed );
		void rotateRoll( float speed );
		
		void lookAt( glm::vec3 target );
		
  private:
  
		glm::vec3 _pos;
		glm::quat _rot;
		
		float _pitchAngle;
		float _yawAngle;
		float _rollAngle;
		
		glm::quat	_pitchQuat;
		glm::quat _yawQuat;
		glm::quat _rollQuat;
		
		glm::vec3 getForwardDirection();
		glm::vec3 getStrafeDirection();
		
  protected:

};

#endif
