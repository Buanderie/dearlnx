
// C
#include <cmath>

// STL
#include <algorithm>
#include <iostream>

// INTERNAL
#include "camera.hpp"

using namespace std;

Camera::Camera()
{
	_pitchAngle = 0.0f;
	_yawAngle = 0.0f;
	_rollAngle = 0.0f;
	_pos = glm::vec3( 0, 0, -2 ); 
}

Camera::~Camera()
{

}

glm::mat4 Camera::getOrientationMatrix()
{
	glm::quat finalRot;
	_pitchQuat = glm::quat( glm::radians(_pitchAngle), 1.0f, 0.0f, 0.0f );
	_yawQuat = glm::quat( glm::radians(_yawAngle), 0.0f, 1.0f, 0.0 );
	_rollQuat = glm::quat( glm::radians(_rollAngle), 0.0f, 0.0f, 1.0f );
	
	finalRot = _pitchQuat * _yawQuat * _rollQuat;
	
	return glm::toMat4(finalRot);
}

glm::vec3 Camera::getForwardDirection()
{
	glm::vec3 res;
	glm::mat4 rotm = getOrientationMatrix();
	res[0] = rotm[2][0];
	res[1] = rotm[2][1];
	res[2] = rotm[2][2];
	return res;
}

glm::vec3 Camera::getStrafeDirection()
{
	glm::vec3 res;
	glm::mat4 rotm = getOrientationMatrix();
	res[0] = rotm[0][0];
	res[1] = rotm[0][1];
	res[2] = rotm[0][2];
	return res;
}

glm::vec3 Camera::getPosition()
{
	return _pos;
}

void Camera::forward( float speed )
{
	glm::vec3 dir = getForwardDirection();
	dir *= speed;
	_pos += dir;
}

void Camera::strafe( float speed )
{
	glm::vec3 dir = getStrafeDirection();
	dir *= speed;
	_pos += dir;
}

void Camera::rotatePitch( float speed )
{
	_pitchAngle += speed;
	if( _pitchAngle >= 360.0f )
	{
		_pitchAngle -= 360.0f;
	}
	else if( _pitchAngle < -360.0f )
	{
		_pitchAngle += 360.0f;
	}
}

void Camera::rotateYaw( float speed )
{
	_yawAngle += speed;
	if( _yawAngle >= 360.0f )
	{
		_yawAngle -= 360.0f;
	}
	else if( _yawAngle < -360.0f )
	{
		_yawAngle += 360.0f;
	}
	cout << "_yawAngle=" << _yawAngle << endl;
	glm::mat4 value = getOrientationMatrix();
  for( int i = 0; i < 4; ++i )
  {
  	for( int j = 0; j < 4; ++j )
  	{
  		cout << value[i][j] << ";";
  	}
  	cout << endl;
  }
  
}

void Camera::rotateRoll( float speed )
{
	_rollAngle += speed;
}

