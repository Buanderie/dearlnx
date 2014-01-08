
// STL
#include <fstream>
#include <iostream>

// INTERNAL
#include "spline.hpp"

using namespace std;

Spline::Spline()
{
	init();
}

Spline::~Spline()
{
	destroy();
}

void Spline::init()
{

}

void Spline::destroy()
{

}

void Spline::addKeyPoint( float t, glm::vec3 p )
{
	keypoint_t kp;
	kp.t = t;
	kp.p = p;
	_keypoints.push_back( kp );
	
/*
	// Duplicate if first keypoint
	if( _keypoints.size() == 1 )
		_keypoints.push_back( kp );
		
	// Duplicate if last keypoint
	if( _keypoints.size() == 5 )
		_keypoints.push_back( kp );
		
	if( _keypoints.size() > 5 )
	{
		_keypoints.erase( _keypoints.end() );
		_keypoints.push_back( kp );
	}
*/
}

glm::vec3 Spline::pointOnCurve( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t )
{
	glm::vec3 result;
	
	float t0 = ((-t + 2.f) * t - 1.f) * t * 0.5f;
	float t1 = (((3.f * t - 5.f) * t) * t + 2.f) * 0.5f;
	float t2 = ((-3.f * t + 4.f) * t + 1.f) * t * 0.5f;
	float t3 = ((t - 1.f) * t * t) * 0.5f;
	
	result.x = p0.x * t0 + p1.x * t1 + p2.x * t2 + p3.x * t3;
	result.y = p0.y * t0 + p1.y * t1 + p2.y * t2 + p3.y * t3;
	result.z = p0.z * t0 + p1.z * t1 + p2.z * t2 + p3.z * t3;
	
	return result;
}

void Spline::save( const std::string& filePath )
{
	ofstream ofs( filePath.c_str() );
	ofs << _keypoints.size() << endl;
	for( int k = 0; k < _keypoints.size(); ++k )
	{
		ofs << _keypoints[ k ].t << endl;
		ofs << _keypoints[ k ].p.x << endl;
		ofs << _keypoints[ k ].p.y << endl;
		ofs << _keypoints[ k ].p.z << endl;
	}
}

void Spline::load( const std::string& filePath )
{
	ifstream ifs( filePath.c_str() );
	int nKeypoints;
	float t;
	float x;
	float y;
	float z;
	ifs >> nKeypoints;
	_keypoints.clear();
	for( int k = 0; k < nKeypoints; ++k )
	{
		ifs >> t >> x >> y >> z;
		keypoint_t kp;
		kp.t = t;
		kp.p = glm::vec3( x, y, z );
		_keypoints.push_back( kp );
	}
	
}

glm::vec3 Spline::getPointFromSync( float t )
{
	glm::vec3 res( 0.0, 0.0, 0.0 );
	
	// Look for the right point...
	int c = -1;
	for( int k = 1; k < _keypoints.size() - 2; ++k )
	{
		cout << "_keypoints[k].t = " << _keypoints[k].t << endl;
		if( _keypoints[k].t < t && _keypoints[k+1].t >= t )
		{	
			c = k;
			break;
		}
	}
	
	if( c < 0 )
		return res;
	else
	{
		float nt = (t - _keypoints[c].t)/(_keypoints[c+1].t-_keypoints[c].t);
		cout << " ####### NT=" << nt << " ####### " << endl;
		res = pointOnCurve( _keypoints[c-1].p, _keypoints[c].p, _keypoints[c+1].p, _keypoints[c+2].p, nt );
		return res;
	}
}

		
