
// STL
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

// windows ?
#include <Windows.h>

// OPENGL
#include <GL/glew.h>
#include <GL/GL.h>


// INTERNAL
#include "shader.hpp"

using namespace std;

Shader::Shader( const std::string& vertexShaderFile, const std::string& fragmentShaderFile )
{
	// First, create the shader program
	prog = glCreateProgram();

	// Load vertex program from file
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	compileShaderFromSource( vertex_shader, vertexShaderFile );

	// Load fragment program from file
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShaderFromSource( fragment_shader, fragmentShaderFile );

	// Attach both FS and VS
	glAttachShader(prog, vertex_shader);
	glAttachShader(prog, fragment_shader);
	glLinkProgram(prog);
}

GLint Shader::compileShaderFromSource( GLuint shader_id, const std::string& shaderSourceFile )
{
	GLint res;
	int infoLogLength;

	string shaderSrc;
	ifstream ifs( shaderSourceFile.c_str() );
	if( ifs.bad() )
	{
		cout << "can't load " << shaderSourceFile << endl;
	}

	while(1)
	{
		if( ifs.eof() )
			break;
		string dump;
		getline( ifs, dump );
		cout << "loaded shader line \"" << dump << endl;
		shaderSrc += dump + "\n";
	}
	const char* ss = (const char*)shaderSrc.c_str();
	glShaderSource( shader_id, 1, &ss, NULL);

	glCompileShader( shader_id );

	// Check shader compilation
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &res);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
    std::vector<GLchar> vshErrMsg(infoLogLength + 1);
    glGetShaderInfoLog(shader_id, infoLogLength, NULL, &vshErrMsg[0]);
    printf("%s\n", &vshErrMsg[0]);
  }

	return shader_id;
}

void Shader::bind()
{
	glUseProgram(prog);
}

void Shader::unbind()
{
	glUseProgram(0);
}
