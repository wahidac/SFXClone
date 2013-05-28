/**
 * @file GLJoe/GLJoe.h
 * 
 * @brief GLJoe main header file
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/11/2013
 */
 
#ifndef DEF_GLJOE_GLJOE_H
#define DEF_GLJOE_GLJOE_H 
 
// Include system headers
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdarg>
 
#ifdef __APPLE__  //apple 
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#elif __linux__ //linux 
#  include "GL/glew.h"
#  include "GL/freeglut.h"
#else //windows
#  include <windows.h> 
#  include "GL/glew.h"
#  include "GL/freeglut.h"
#endif // __APPLE__

namespace GLJoe
{
	
}

#include "./common/error.h"
#include "./common/utils.h"
#include "./utils/initshader.h"
#include "./utils/tgaimage.h"
#include "./core/vec2.h"
#include "./core/vec3.h"
#include "./core/vec4.h"
#include "./core/mat2.h"
#include "./core/mat3.h"
#include "./core/mat4.h"
#include "./core/stack.h"
#include "./geometry/transform.h"
#include "./geometry/transform2d.h"
#include "./geometry/projection.h"
#include "./geometry/cube.h"


#endif // DEF_GLJOE_GLJOE_H
