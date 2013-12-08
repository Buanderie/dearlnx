#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <GL/glew.h>

using namespace std;

bool gl_check_error(int line, const char* comment);
#define CHECKERROR gl_check_error(__LINE__,__FUNCTION__);

#endif