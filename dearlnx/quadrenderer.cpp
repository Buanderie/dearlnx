
// STL
#include <iostream>

// GLEW
#include <GL/glew.h>

// INTERNAL
#include "quadrenderer.h"
#include "utils.hpp"

using namespace std;

void QuadRenderer::init()
{
	if (!GLEW_ARB_vertex_array_object)
      std::cerr << "ARB_vertex_array_object not available." << std::endl;

    static const GLfloat vao_data[] = {
  -1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
  1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,

  1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,
  -1.0f, -1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
};

    glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vao_data), vao_data, GL_STATIC_DRAW);
}

void QuadRenderer::destroy()
{

}

void QuadRenderer::draw()
{
	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}