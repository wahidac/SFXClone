#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "GLJoe/GLJoe.h"
#include "ObjLoader/OBJObject.h"
#include "ImgLoader.h"

using namespace GLJoe;

#define IMG_WIDTH 1920
#define IMG_HEIGHT 1200


class Background
{
public:
	Background(char* backImage, OBJObjectShaderHandles &shaderHandles);
	void draw();

	void resize(int width, int height);
<<<<<<< HEAD
=======
	void moveGroundTexture(int offset);
>>>>>>> parent of 96d99d3... Kyle - finished background by adding moving buildings to scenery

private:
	Vec4* plane_vertices;
	Vec2* tex_coords;
	Vec3* plane_normals;
	Vec4* material_ambient;
	Vec4* material_other;

	GLubyte image[IMG_WIDTH * IMG_HEIGHT * 3];

	Transform cMw;
	Transform wMo;

	OBJObjectShaderHandles handles;

	ImgLoader imgLoader;

	GLuint vao;

	int num_vertices;
<<<<<<< HEAD
=======
	int texture_offset;
>>>>>>> parent of 96d99d3... Kyle - finished background by adding moving buildings to scenery

	GLuint textures[1];

	void initializeOpenGLBuffers();
};

#endif