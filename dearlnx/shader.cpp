
// STL
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

// windows ?
#ifdef _WIN32
#include <Windows.h>
#endif

// OPENGL
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>

#ifdef _WIN32
#include <GL/GL.h>
#else
#include <GL/gl.h>
#endif

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

bool Shader::setUniform1f( const std::string& name, float value )
{
  bool ret = false;
  glUseProgram(prog);
  if( _uniforms.find( name ) == _uniforms.end() )
  {
    GLint uniret = glGetUniformLocation( prog, name.c_str() );
    if( uniret >= 0 )
    {
      _uniforms.insert( make_pair( name, uniret ) );
      ret = true;
    }
    else
      return false;
  }
  else
  {
    ret = true;
  }

  glUniform1f ( _uniforms[name], value );
  return ret;
}

bool Shader::setUniformVec3( const std::string& name, glm::vec3 value )
{
  bool ret = false;
  glUseProgram(prog);
  if( _uniforms.find( name ) == _uniforms.end() )
  {
    GLint uniret = glGetUniformLocation( prog, name.c_str() );
    if( uniret >= 0 )
    {
      _uniforms.insert( make_pair( name, uniret ) );
      ret = true;
    }
    else
      return false;
  }
  else
  {
    ret = true;
  }

  glUniform3f ( _uniforms[name], value.x, value.y, value.z );
  return ret;
}

bool Shader::setUniformMat4( const std::string& name, glm::mat4 value )
{
  bool ret = false;
  glUseProgram(prog);
  if( _uniforms.find( name ) == _uniforms.end() )
  {
    GLint uniret = glGetUniformLocation( prog, name.c_str() );
    if( uniret >= 0 )
    {
      _uniforms.insert( make_pair( name, uniret ) );
      ret = true;
    }
    else
      return false;
  }
  else
  {
    ret = true;
  }

  glUniformMatrix4fv ( _uniforms[name], 1, GL_FALSE, &value[0][0] );

  return ret;
}

bool Shader::setTextureSampler( const int samplerId, const Texture2D& texture )
{
  bool ret = false;
  glUseProgram(prog);
  stringstream sstr;  
  sstr << "sampler" << samplerId;
  string name = sstr.str();
  if( _uniforms.find( name ) == _uniforms.end() )
  {
    GLint uniret = glGetUniformLocation( prog, name.c_str() );
    if( uniret >= 0 )
    {
      _uniforms.insert( make_pair( name, uniret ) );
      ret = true;
    }
    else
      return false;
  }
  else
  {
    ret = true;
  } 
  glUniform1i( _uniforms[name], samplerId );
  glActiveTexture(GL_TEXTURE0 + samplerId);
  glBindTexture(GL_TEXTURE_2D, texture.id()); 
}
