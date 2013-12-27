#ifndef __SHADER_HPP__
#define __SHADER_HPP__

// STL
#include <string>
#include <map>

// GLM
#include <glm/glm.hpp>

// INTERNAL
#include "texture.h"

class Shader
{
public:
	Shader( const std::string& vertexShaderFile, const std::string& fragmentShaderFile );
	virtual ~Shader(){}

	void bind();
	void unbind();

  bool setUniform1f( const std::string& name, float value );  
  bool setUniformVec3( const std::string& name, glm::vec3 value );
  bool setUniformMat4( const std::string& name, glm::mat4 value );
  bool setTextureSampler( const int samplerId, const Texture2D& texture );
  
private:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint prog;
	
  std::map< std::string, GLuint > _uniforms;

	GLint compileShaderFromSource( GLuint shader_id, const std::string& shaderSourceFile );

protected:

};

#endif
