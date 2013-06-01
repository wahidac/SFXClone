#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "GLJoe.h"
#include "OBJObject.h"
#include "ImgLoader.h"

using namespace GLJoe;

#define IMG_WIDTH 2048
#define IMG_HEIGHT 2048


class Background
{
public:
	Background(char* backImage, char* groundImage, OBJObjectShaderHandles &shaderHandles);
	void draw();

	void resize(int width, int height);
	void moveGroundTexture(int offset);

private:
	Vec4* plane_vertices;
	Vec2* tex_coords;
	Vec3* plane_normals;
	Vec4* material_ambient;
	Vec4* material_other;

	GLubyte back_image[IMG_WIDTH * IMG_HEIGHT * 3];
	GLubyte ground_image[IMG_WIDTH * IMG_HEIGHT * 3];

	Transform back_cMw;
	Transform back_wMo;
	Transform ground_cMw;
	Transform ground_wMo;

	OBJObjectShaderHandles handles;

	ImgLoader imgLoader;

	OBJObject* city;

	GLuint vao;

	int num_vertices;
	int texture_offset;

	GLuint textures[2];

	void initializeOpenGLBuffers();
};

#endif