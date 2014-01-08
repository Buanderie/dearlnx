#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

// STL
#include <vector>
#include <string>

// GLM
#include <glm/glm.hpp>

typedef struct
{
	float t;
	glm::vec3 p;
} keypoint_t;

class Spline
{
	public:
		Spline();
		virtual ~Spline();
		
		void addKeyPoint( float t, glm::vec3 p );
		void save( const std::string& filePath );
		void load( const std::string& filePath );
		
		glm::vec3 getPointFromSync( float t );
		
	private:
		std::vector< keypoint_t > _keypoints;
		
		void init();
		void destroy();
		
		glm::vec3 pointOnCurve( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t );
		
	protected:
	
};

#endif
