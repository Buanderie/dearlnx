#ifndef __QUADRENDERER_HPP__
#define __QUADRENDERER_HPP__

class QuadRenderer
{
public:
	QuadRenderer(){ init(); };
	virtual ~QuadRenderer(){ destroy(); }
	void draw();

private:
	GLuint _vao;
	GLuint _vbo;

	void init();
	void destroy();
	GLfloat* vertices;

protected:

};

#endif