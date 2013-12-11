#ifndef __SHADER_HPP__
#define __SHADER_HPP__

// STL
#include <string>
#include <map>

class Shader
{
public:
	Shader( const std::string& vertexShaderFile, const std::string& fragmentShaderFile );
	virtual ~Shader(){}

	void bind();
	void unbind();

  bool setUniform1f( const std::string& name, float value );  
  
private:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint prog;
	
  std::map< std::string, GLuint > _uniforms;

	GLint compileShaderFromSource( GLuint shader_id, const std::string& shaderSourceFile );

protected:

};

#endif
