#ifndef __SHADER_HPP__
#define __SHADER_HPP__

// STL
#include <string>

class Shader
{
public:
	Shader( const std::string& vertexShaderFile, const std::string& fragmentShaderFile );
	virtual ~Shader(){}

	void bind();
	void unbind();

private:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint prog;
	
	GLint compileShaderFromSource( GLuint shader_id, const std::string& shaderSourceFile );

protected:

};

#endif