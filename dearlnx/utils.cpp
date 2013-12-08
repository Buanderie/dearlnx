// INTERNAL
#include "utils.hpp"

bool gl_check_error(int line, const char* comment)
{
    GLuint err = glGetError();
	
    if( err != GL_NO_ERROR )
    {
        cout << "GL error at line " <<line<<" of "<< comment << ": " <<  err << " ie: " << gluErrorString(err) <<endl;
        return false;
    }
    return true;
}