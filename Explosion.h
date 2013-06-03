//
//  Explosion.h
//  particles
//
//  Created by Wahid Chowdhury on 5/31/13.
//  Copyright (c) 2013 Wahid Chowdhury. All rights reserved.
//

#ifndef __particles__Explosion__
#define __particles__Explosion__

#include <iostream>
#include "GLJoe/GLJoe.h"
#include "OBJLoader/OBJObject.h"
#include "ImgLoader.h"

using namespace GLJoe;

#define IMG_WIDTH 420
#define IMG_HEIGHT 420

class Explosion
{
public:
	Explosion(char* explosionImage, OBJObjectShaderHandles &shaderHandles);
	void draw();

    Transform cMw;
	Transform wMo;

private:
	Vec4* plane_vertices;
	Vec2* tex_coords;
	Vec3* plane_normals;
	Vec4* material_ambient;
	Vec4* material_other;
    
	GLubyte image[IMG_WIDTH * IMG_HEIGHT * 3];
    
    
	OBJObjectShaderHandles handles;
    
	ImgLoader imgLoader;
    
	GLuint vao;
    
	int num_vertices;
    
	GLuint textures[1];
    
	void initializeOpenGLBuffers();
};


#endif /* defined(__particles__Explosion__) */
