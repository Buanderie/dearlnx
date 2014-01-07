
// C
#include <cmath>

// STL
#include <algorithm>
#include <iostream>

// INTERNAL
#include "camera.hpp"
#include "quaternion_utils.hpp"

using namespace std;

Camera::Camera()
{
	_pitchAngle = 0.0f;
	_yawAngle = 0.0f;
	_rollAngle = 0.0f;
	_pos = glm::vec3( 50, 0, -50 ); 
}

Camera::~Camera()
{

}

void Camera::update()
{
	_pitchQuat = glm::angleAxis( (_pitchAngle), 1.0f, 0.0f, 0.0f );
	_yawQuat = glm::angleAxis( (_yawAngle), 0.0f, 1.0f, 0.0f );
	_rollQuat = glm::angleAxis( (_rollAngle), 0.0f, 0.0f, 1.0f );
	_rot = _yawQuat * _pitchQuat * _rollQuat;
}

glm::mat4 Camera::getOrientationMatrix()
{
	glm::normalize( _rot );
	return glm::toMat4( _rot );
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

glm::vec3 Camera::getViewVector()
{
	return getForwardDirection();
}

glm::vec3 Camera::getUpVector()
{
	glm::vec3 res;
	glm::mat4 rotm = getOrientationMatrix();
	res[0] = rotm[1][0];
	res[1] = rotm[1][1];
	res[2] = rotm[1][2];
	return res;
}

glm::vec3 Camera::getRightVector()
{
	return getStrafeDirection();
}

/*
quat RotationBetweenVectors(vec3 start, vec3 dest){
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (gtx::norm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return gtx::quaternion::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return quat(
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}
*/
void Camera::lookAt( glm::vec3 target )
{
	
	glm::vec3 dest = glm::normalize( target - _pos );		// dest
	glm::vec3 start = glm::normalize( getForwardDirection() );	// start
	glm::quat dQuat = RotationBetweenVectors( start, dest);
	_rot = LookAt( dest, glm::vec3( 0.0, 1.0, 0.0 ) );
	
}

